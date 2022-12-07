#ifndef _CONTROLLER_CLASS_H
#define _CONTROLLER_CLASS_H

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "sensorClass.hpp"
#include <map>

#include "sensor/sensorBMP280.hpp"
#include "sensor/sensorLight.hpp"
#include "sensor/sensorFlame.hpp"
#include "sensor/sensorO2.hpp"
#include "sensor/sensorCO2.hpp"
#include "sensor/sensorSunlight.hpp"

#define CONTROLLER_DEF_BAUD 9600
#define CONTROLLER_DEF_SLAVE 1
#define CONTROLLER_DEF_VERSION 0x10000000

#define CONTROLLER_DEF_VALUE 0x0000
#define CONTROLLER_RS485_PORT SERIAL_PORT_HARDWARE
#define CONTROLLER_RS485_TX_PIN 1
#define CONTROLLER_RS485_DE_PIN D8
#define CONTROLLER_RS485_RE_PIN D8

RS485Class m_RS485(CONTROLLER_RS485_PORT, CONTROLLER_RS485_TX_PIN, CONTROLLER_RS485_DE_PIN, CONTROLLER_RS485_RE_PIN);
ModbusRTUServerClass m_ModbusRTUServer(RS485);

class controllerClass
{
private:
    /* data */
    uint8_t _slave;
    uint32_t _baudrate;
    uint16_t _regs;

    std::map<uint16_t, sensorClass *> m_sensorMap;

    enum
    {
        REG_ADDR = 0,
        REG_BAUD,
        REG_VERSION,
    };

    ModbusRTUServerClass *_ModbusRTUServer;

public:
    controllerClass(ModbusRTUServerClass &ModbusRTUServer = m_ModbusRTUServer) : _ModbusRTUServer(&ModbusRTUServer), _regs(4){};
    ~controllerClass(){};
    uint16_t addSensor(sensorClass *sensor);
    // bool removeSensor(sensorClass *sensor);
    bool begin(uint8_t slave = CONTROLLER_DEF_SLAVE, uint32_t baudrate = CONTROLLER_DEF_BAUD);
    int poll();
    uint16_t size();
};

bool controllerClass::begin(uint8_t slave, uint32_t baudrate)
{
    pinMode(GROVE_SWITCH_PIN, OUTPUT);

    _slave = slave;
    _baudrate = baudrate;

    // start the Modbus RTU server, with (slave) id 1

    if (!_ModbusRTUServer->begin(_slave, _baudrate))
    {
        Serial.println("Failed to start Modbus RTU Client!");
        while (1)
            ;
    }

    Serial.print("Version: ");
    Serial.println(CONTROLLER_DEF_VERSION, HEX);

    Serial.print("regs: ");
    Serial.println(_regs);

    _ModbusRTUServer->configureInputRegisters(0x00, _regs);
    _ModbusRTUServer->configureHoldingRegisters(0x00, _regs);

    _ModbusRTUServer->inputRegisterWrite(controllerClass::REG_ADDR, _slave);
    _ModbusRTUServer->holdingRegisterWrite(controllerClass::REG_ADDR, _slave);

    _ModbusRTUServer->inputRegisterWrite(controllerClass::REG_BAUD, _baudrate / 100);
    _ModbusRTUServer->holdingRegisterWrite(controllerClass::REG_BAUD, _baudrate / 100);

    _ModbusRTUServer->inputRegisterWrite(controllerClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    _ModbusRTUServer->inputRegisterWrite(controllerClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));
    _ModbusRTUServer->holdingRegisterWrite(controllerClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    _ModbusRTUServer->holdingRegisterWrite(controllerClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));

    /* skip head information */
    for (uint16_t i = 4; i < _regs; i += 1)
    {
        _ModbusRTUServer->inputRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
        _ModbusRTUServer->holdingRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
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
        Serial.println(sensor->name().c_str());
        for (auto m_iter = m_measureValue.begin(); m_iter != m_measureValue.end(); ++m_iter)
        {
            Serial.print(m_iter->addr);
            Serial.print(':');
            switch (m_iter->type)
            {
            case sensorClass::regType_t::REG_TYPE_U16_AB:
                Serial.println(m_iter->value.u16);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, m_iter->value.u16);
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, m_iter->value.u16);
                break;
            case sensorClass::regType_t::REG_TYPE_S16_AB:
                Serial.println(m_iter->value.s16);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, m_iter->value.s16);
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, m_iter->value.s16);
                break;
            case sensorClass::regType_t::REG_TYPE_U32_ABCD:
                Serial.println(m_iter->value.u32);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 >> 16));
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 >> 16));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                break;
            case sensorClass::regType_t::REG_TYPE_S32_ABCD:
                Serial.println(m_iter->value.s32);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 >> 16));
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 >> 16));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                break;
            case sensorClass::regType_t::REG_TYPE_U32_CDAB:
                Serial.println(m_iter->value.u32);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 >> 16));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.u32 & 0x0000FFFF));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.u32 >> 16));
                break;
            case sensorClass::regType_t::REG_TYPE_S32_CDAB:
                Serial.println(m_iter->value.s32);
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                _ModbusRTUServer->inputRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 >> 16));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr, (uint16_t)(m_iter->value.s32 & 0x0000FFFF));
                _ModbusRTUServer->holdingRegisterWrite(m_iter->addr + 1, (uint16_t)(m_iter->value.s32 >> 16));
                break;
            default:
                break;
            }
        }
    }

    return _ModbusRTUServer->poll();
}

uint16_t controllerClass::size()
{
    return m_sensorMap.size();
}

uint16_t controllerClass::addSensor(sensorClass *_sensor)
{
    uint16_t regs = 0;

    for (std::map<uint16_t, sensorClass *>::const_iterator iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
    {
        if (iter->second == _sensor)
        {
            return false;
        }
    }

    m_sensorMap.insert(std::pair<uint16_t, sensorClass *>(m_sensorMap.size(), _sensor));

    regs = _sensor->init(_regs);

    _regs += regs;

    return regs;
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
