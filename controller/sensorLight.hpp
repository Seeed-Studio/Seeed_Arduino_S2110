#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include "sensorClass.hpp"
#include <Wire.h>

#define LIGHT_ADC_PIN A3

class sensorLight : public sensorClass
{
public:
    sensorLight() : sensorClass("Light V1.2"){};
    ~sensorLight(){};
    virtual bool init();
    virtual bool connected();
    virtual bool sample();

private:
    bool _connected;
};

bool sensorLight::init()
{

    m_measureValueMap[sensorClass::REG_LIGHT] = 0;

    _connected = true;
    return true;
}

bool sensorLight::sample()
{
    GROVE_SWITCH_ADC;

    int32_t value = analogRead(LIGHT_ADC_PIN);

    m_measureValueMap[sensorClass::REG_LIGHT] = value * SCALE;

    return true;
}

bool sensorLight::connected()
{
    return _connected;
}

#endif