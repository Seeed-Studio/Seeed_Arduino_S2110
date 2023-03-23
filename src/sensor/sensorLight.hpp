#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include "sensorClass.hpp"

#define LIGHT_ADC_PIN A3

class sensorLight : public sensorClass
{
public:
    sensorLight() : sensorClass("Light V1.2"){};
    ~sensorLight(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        LIGHT = 0,
        MAX
    };
};

uint16_t sensorLight::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorLight::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        t_reg += sensorClass::valueLength(value.type);
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
    }

    _connected = true;
    return t_reg - reg;
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