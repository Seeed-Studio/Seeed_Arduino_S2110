#ifndef _SENSOR_UV_H
#define _SENSOR_UV_H

#include "sensorClass.hpp"

class sensorUV : public sensorClass
{
public:
    sensorUV() : sensorClass("UV V1.1"){};
    ~sensorUV(){};

    uint16_t init(uint16_t reg);
    bool connected();
    bool sample();

    enum
    {
        UV_INDEX = 0,
        MAX
    };
};

uint16_t sensorUV::init(uint16_t reg)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorUV::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        t_reg += sensorClass::valueLength(value.type);
        m_valueVector.emplace_back(value);
    }

    _connected = true;
    return t_reg - reg;
}

bool sensorUV::sample()
{
    GROVE_SWITCH_ADC;

    float value = (analogRead(SENSOR_ANALOG_PIN)*1000/4.3 - 83)/21;

    m_valueVector[sensorUV::UV_INDEX].value.s32 = (int)(value * SCALE);

    return true;
}

bool sensorUV::connected()
{
    return _connected;
}

#endif