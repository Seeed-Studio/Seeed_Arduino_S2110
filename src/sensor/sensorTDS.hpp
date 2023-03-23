#ifndef _SENSOR_TDS_H
#define _SENSOR_TDS_H

#include "sensorClass.hpp"

class sensorTDS : public sensorClass
{
public:
    sensorTDS() : sensorClass("TDS"){};
    ~sensorTDS(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        TDS = 0,
        MAX
    };
};

uint16_t sensorTDS::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorTDS::MAX; i++)
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

bool sensorTDS::sample()
{
    GROVE_SWITCH_ADC;

    // unit : ppm
    float vol = analogRead(SENSOR_ANALOG_PIN) * (5.0 / 1024.0);
    float value = (133.42 / vol * vol * vol - 255.86 * vol * vol + 857.39 * vol) * 0.5;

    m_valueVector[sensorTDS::TDS].value.s32 = (int)(value * SCALE);

    return true;
}

bool sensorTDS::connected()
{
    return _connected;
}

#endif