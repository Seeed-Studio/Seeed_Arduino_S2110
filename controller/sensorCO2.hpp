#ifndef _SENSOR_CO2_H
#define _SENSOR_CO2_H
#include "sensorClass.hpp"
#include <SensirionI2CScd4x.h>
#include <Wire.h>

class sensorCO2 : public sensorClass
{
public:
    sensorCO2() : sensorClass("CO2"){};
    ~sensorCO2(){};
    virtual bool init();
    virtual bool connected();
    virtual bool sample();

private:
    SensirionI2CScd4x _scd4x; // IIC
    bool _connected;
};

bool sensorCO2::init()
{
    GROVE_SWITCH_IIC;

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    uint32_t timeout = 5000;

    Wire.begin();

    _scd4x.begin(Wire);

    if (_scd4x.stopPeriodicMeasurement())
    {
        _connected = false;
        return false;
    }

    if (_scd4x.getSerialNumber(serial0, serial1, serial2))
    {
        _connected = false;
        return false;
    }

    if (_scd4x.startPeriodicMeasurement())
    {
        _connected = false;
        return false;
    }

    m_measureValueMap[sensorClass::REG_CO2_CO2] = 0;
    m_measureValueMap[sensorClass::REG_CO2_HUMIDITY] = 0;
    m_measureValueMap[sensorClass::REG_CO2_TEMPE] = 0;

    _connected = true;

    return true;
}

bool sensorCO2::sample()
{
    GROVE_SWITCH_IIC;
    uint16_t error = 0;
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;

    error = _scd4x.readMeasurement(co2, temperature, humidity);

    if (error == 0)
    {
        m_measureValueMap[sensorClass::REG_CO2_CO2] = co2 * SCALE;
        m_measureValueMap[sensorClass::REG_CO2_HUMIDITY] = humidity * SCALE;
        m_measureValueMap[sensorClass::REG_CO2_TEMPE] = temperature * SCALE;
    }

    return true;
}

bool sensorCO2::connected()
{
    return _connected;
}

#endif