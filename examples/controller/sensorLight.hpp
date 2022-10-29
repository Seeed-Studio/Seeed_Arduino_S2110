#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include "sensorClass.hpp"

#define LIGHT_ADC_PIN A3

class sensorLight : public sensorClass
{
public:
    sensorLight() : sensorClass("Light V1.2"){};
    ~sensorLight(){};

        virtual bool init();
    virtual bool connected();
    virtual bool sample();

    enum
    {
        LIGHT = 0,
        MAX = 0
    };
};

bool sensorLight::init()
{
    _connected = true;

    for (uint16_t i = 0; i < sensorLight::MAX; i++)
    {
        sensorClass::reg_t value;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.push_back(value);
    }

    return true;
}

bool sensorLight::sample()
{
    GROVE_SWITCH_ADC;

    int32_t value = analogRead(LIGHT_ADC_PIN);

    m_valueVector[sensorLight::LIGHT].value.s32 = value * SCALE;

    return true;
}

bool sensorLight::connected()
{
    return _connected;
}

#endif