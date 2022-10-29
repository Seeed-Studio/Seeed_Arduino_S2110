#ifndef _CONTROLLER_CLASS_H
#define _CONTROLLER_CLASS_H

#include <ArduinoModbus.h>
#include "sensorClass.hpp"
#include <map>

#define CONTROLLER_DEF_BAUD 9600
#define CONTROLLER_DEF_SLAVE 1
#define CONTROLLER_DEF_VERSION 0x10000000
#define CONTROLLER_DEF_VALUE 0x0000

class controllerClass
{
private:
    /* data */
    uint8_t _slave;
    uint32_t _baudrate;
    uint32_t _regs;

    std::map<uint16_t, sensorClass *> m_sensorMap;

    enum
    {
        REG_ADDR = 0,
        REG_BAUD,
        REG_VERSION,
    };

public:
    controllerClass(uint8_t slave = CONTROLLER_DEF_SLAVE, uint32_t baudrate = CONTROLLER_DEF_BAUD) : _slave(slave), _baudrate(baudrate){};
    ~controllerClass(){};
    bool addSensor(sensorClass *sensor);
    // bool removeSensor(sensorClass *sensor);
    bool begin();
    int poll();
    uint16_t size();
};

bool controllerClass::begin()
{
    pinMode(GROVE_SWITCH_PIN, OUTPUT);

    // start the Modbus RTU server, with (slave) id 1

    if (!ModbusRTUServer.begin(_slave, _baudrate))
    {
        Serial.println("Failed to start Modbus RTU Client!");
        while (1)
            ;
    }

    Serial.print("Version: ");
    Serial.println(CONTROLLER_DEF_VERSION, HEX);

    _regs += 4;

    ModbusRTUServer.configureInputRegisters(0x00, _regs);
    ModbusRTUServer.configureHoldingRegisters(0x00, _regs);

    ModbusRTUServer.inputRegisterWrite(controllerClass::REG_ADDR, _slave);
    ModbusRTUServer.holdingRegisterWrite(controllerClass::REG_ADDR, _slave);

    ModbusRTUServer.inputRegisterWrite(controllerClass::REG_BAUD, _baudrate / 100);
    ModbusRTUServer.holdingRegisterWrite(controllerClass::REG_BAUD, _baudrate / 100);

    ModbusRTUServer.inputRegisterWrite(controllerClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    ModbusRTUServer.inputRegisterWrite(controllerClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));
    ModbusRTUServer.holdingRegisterWrite(controllerClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    ModbusRTUServer.holdingRegisterWrite(controllerClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));

    /* skip head information */
    for (uint16_t i = 4; i < _regs; i += 1)
    {
        ModbusRTUServer.inputRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
        ModbusRTUServer.holdingRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
    }

    return true;
}

int controllerClass::poll()
{
    for (auto iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
    {
        sensorClass *sensor = (sensorClass *)iter->second;

        if (!sensor->connected())
        {
            continue;
        }

        sensor->sample();

        auto m_measureValue = sensor->getMeasureValue();
        // Serial.println(sensor->name().c_str());
        for (auto m_iter = m_measureValue.begin(); m_iter != m_measureValue.end(); ++m_iter)
        {
            Serial.print(m_iter->addr);
            Serial.print(':');
            switch (m_iter->type)
            {
            case sensorClass::regType_t::REG_TYPE_U16_AB:
                // Serial.println(m_iter->value.u16);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, m_iter->value.u16);
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, m_iter->value.u16);
            case sensorClass::regType_t::REG_TYPE_S16_AB:
                // Serial.println(m_iter->value.s16);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, m_iter->value.s16);
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, m_iter->value.s16);
            case sensorClass::regType_t::REG_TYPE_U32_ABCD:
                // Serial.println(m_iter->value.u32);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 >> 16));
                ModbusRTUServer.inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 >> 16));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
            case sensorClass::regType_t::REG_TYPE_S32_ABCD:
                // Serial.println(m_iter->value.s32);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 >> 16));
                ModbusRTUServer.inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 >> 16));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
            case sensorClass::regType_t::REG_TYPE_U32_CDAB:
                // Serial.println(m_iter->value.u32);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                ModbusRTUServer.inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 >> 16));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 >> 16));
            case sensorClass::regType_t::REG_TYPE_S32_CDAB:
                // Serial.println(m_iter->value.s32);
                ModbusRTUServer.inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                ModbusRTUServer.inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 >> 16));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                ModbusRTUServer.holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 >> 16));
                break;
            default:
                break;
            }
        }
    }

    return ModbusRTUServer.poll();
}

uint16_t controllerClass::size()
{
    return m_sensorMap.size();
}

bool controllerClass::addSensor(sensorClass *_sensor)
{
    for (std::map<uint16_t, sensorClass *>::const_iterator iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
    {
        if (iter->second == _sensor)
        {
            return false;
        }
    }

    m_sensorMap[size() + 1] = _sensor;
    auto m_measureValue = _sensor->getMeasureValue();
    for (int i = 0; i < m_measureValue.size(); i++)
    {
        m_measureValue[i].addr = _regs;
        _regs += sensorClass::valueLength(m_measureValue[i].type);
    }

    _sensor->init();

    return true;
}

// bool controllerClass::removeSensor(sensorClass *_sensor)
// {
//     for (std::map<uint16_t, sensorClass *>::iterator iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
//     {
//         if (iter->second == _sensor)
//         {
//             iter = m_sensorMap.erase(iter);
//             return true;
//         }
//     }

//     return false;
// }

#endif
