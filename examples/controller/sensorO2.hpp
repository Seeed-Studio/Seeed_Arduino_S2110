#ifndef _SENSOR_O2_H
#define _SENSOR_O2_H

#include "sensorClass.hpp"

#define O2_ADC_PIN A3
#define O2_ADC_VREFER 3.3f

class sensorO2 : public sensorClass
{
public:
    sensorO2() : sensorClass("O2"){};
    ~sensorO2(){};

    virtual bool init();
    virtual bool connected();
    virtual bool sample();

    enum
    {
        O2 = 0,
        MAX
    };
};

bool sensorO2::init()
{
    for (uint16_t i = 0; i < sensorO2::MAX; i++)
    {
        sensorClass::reg_t value;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.push_back(value);
    }

    _connected = true;
    return true;
}

bool sensorO2::sample()
{
    GROVE_SWITCH_ADC;

    uint32_t sum = 0;
    float MeasuredVout = 0;

    for (int i = 0; i < 32; i++)
    {
        sum += analogRead(O2_ADC_PIN);
    }

    sum >>= 5;

    MeasuredVout = sum * (O2_ADC_VREFER / 1023.0);
    MeasuredVout = MeasuredVout * 0.36 / 2.0 * 100;

    m_valueVector[sensorO2::O2].value.s32 = MeasuredVout * SCALE;

    return true;
}

bool sensorO2::connected()
{
    return _connected;
}

#endif