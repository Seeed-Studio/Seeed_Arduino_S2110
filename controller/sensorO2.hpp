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

private:
    bool _connected;
};

bool sensorO2::init()
{

    m_measureValueMap[sensorClass::REG_O2] = 0;

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

    m_measureValueMap[sensorClass::REG_O2] = MeasuredVout * SCALE;

    return true;
}

bool sensorO2::connected()
{
    return _connected;
}

#endif