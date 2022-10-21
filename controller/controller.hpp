#ifndef _CONTROLLER_CLASS_H
#define _CONTROLLER_CLASS_H

#include <ArduinoModbus.h>
#include "sensorClass.hpp"
#include "sensorBMP280.hpp"
#include "sensorLight.hpp"
#include "sensorFlame.hpp"
#include "sensorO2.hpp"
#include "sensorCO2.hpp"
#include "sensorSunlight.hpp"
#include <map>

#define CONTROLLER_DEF_BAUD 9600
#define CONTROLLER_DEF_SLAVE 1
#define CONTROLLER_DEF_VERSION 0x10000000
#define CONTROLLER_DEF_VALUE 0x80000000

class controllerClass
{
private:
    /* data */
    bool addSensor(sensorClass *sensor);
    bool removeSensor(sensorClass *sensor);
    uint8_t _slave;
    uint32_t _baudrate;

    std::map<uint16_t, sensorClass *> m_sensorMap;

public:
    controllerClass(uint8_t slave = CONTROLLER_DEF_SLAVE, uint32_t baudrate = CONTROLLER_DEF_BAUD) : _slave(slave), _baudrate(baudrate){};
    ~controllerClass(){};
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

    ModbusRTUServer.configureInputRegisters(0x00, sensorClass::REG_MAX);
    ModbusRTUServer.configureHoldingRegisters(0x00, sensorClass::REG_MAX);

    ModbusRTUServer.inputRegisterWrite(sensorClass::REG_ADDR, _slave);
    ModbusRTUServer.holdingRegisterWrite(sensorClass::REG_ADDR, _slave);

    ModbusRTUServer.inputRegisterWrite(sensorClass::REG_BAUD, _baudrate / 100);
    ModbusRTUServer.holdingRegisterWrite(sensorClass::REG_BAUD, _baudrate / 100);

    ModbusRTUServer.inputRegisterWrite(sensorClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    ModbusRTUServer.inputRegisterWrite(sensorClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));
    ModbusRTUServer.holdingRegisterWrite(sensorClass::REG_VERSION, (uint16_t)(CONTROLLER_DEF_VERSION >> 16));
    ModbusRTUServer.holdingRegisterWrite(sensorClass::REG_VERSION + 1, (uint16_t)(CONTROLLER_DEF_VERSION & 0x0000FFFF));

    for (uint16_t i = sensorClass::REG_VERSION + 2; i < sensorClass::REG_MAX; i += 2)
    {
        ModbusRTUServer.inputRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
        ModbusRTUServer.inputRegisterWrite(i + 1, (uint16_t)(CONTROLLER_DEF_VALUE & 0x0000FFFF));
        ModbusRTUServer.holdingRegisterWrite(i, (uint16_t)(CONTROLLER_DEF_VALUE >> 16));
        ModbusRTUServer.holdingRegisterWrite(i + 1, (uint16_t)(CONTROLLER_DEF_VALUE & 0x0000FFFF));
    }

    // CO2
    sensorCO2 *co2 = new sensorCO2();
    if (co2->init())
    {
        Serial.println("CO2 sensor init success");
        addSensor(co2);
    }
    else
    {
        Serial.println("Failed to init CO2 sensor!");
        delete co2;
    }

    // light
    sensorLight *light = new sensorLight();
    if (light->init())
    {
        Serial.println("Light sensor init success");
        addSensor(light);
    }
    else
    {
        Serial.println("Failed to init light sensor!");
        delete light;
    }

    // flame
    sensorFlame *flame = new sensorFlame();
    if (flame->init())
    {
        Serial.println("Flame sensor init success");
        addSensor(flame);
    }
    else
    {
        Serial.println("Failed to init flame sensor!");
        delete flame;
    }

    // O2
    sensorO2 *o2 = new sensorO2();
    if (o2->init())
    {
        Serial.println("O2 sensor init success");
        addSensor(o2);
    }
    else
    {
        Serial.println("Failed to init O2 sensor!");
        delete o2;
    }

    // sunlight
    sensorSunlight *sunlight = new sensorSunlight();
    if (sunlight->init())
    {
        Serial.println("Sunlight sensor init success");
        addSensor(sunlight);
    }
    else
    {
        Serial.println("Failed to init sunlight sensor!");
        delete sunlight;
    }

    // bmp280
    sensorBMP280 *bmp280 = new sensorBMP280();
    if (bmp280->init())
    {
        Serial.println("BMP280 sensor init success");
        addSensor(bmp280);
    }
    else
    {
        Serial.println("Failed to init BMP280 sensor!");
        delete bmp280;
    }

    return true;
}

int controllerClass::poll()
{
    for (auto iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
    {
        sensorClass *sensor = (sensorClass *)iter->second;

        sensor->sample();

        auto m_measureValue = sensor->getMeasureValue();
        // Serial.println(sensor->name().c_str());
        for (auto m_iter = m_measureValue.begin(); m_iter != m_measureValue.end(); ++m_iter)
        {
            Serial.print(m_iter->first);
            Serial.print(':');
            Serial.println(m_iter->second);
            ModbusRTUServer.inputRegisterWrite(m_iter->first, (uint16_t)(m_iter->second >> 16));
            ModbusRTUServer.inputRegisterWrite(m_iter->first + 1, (uint16_t)m_iter->second & 0x0000FFFF);
            ModbusRTUServer.holdingRegisterWrite(m_iter->first, (uint16_t)(m_iter->second >> 16));
            ModbusRTUServer.holdingRegisterWrite(m_iter->first + 1, (uint16_t)m_iter->second & 0x0000FFFF);
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
    return true;
}

bool controllerClass::removeSensor(sensorClass *_sensor)
{
    for (std::map<uint16_t, sensorClass *>::iterator iter = m_sensorMap.begin(); iter != m_sensorMap.end(); ++iter)
    {
        if (iter->second == _sensor)
        {
            iter = m_sensorMap.erase(iter);
            return true;
        }
    }

    return false;
}

#endif
