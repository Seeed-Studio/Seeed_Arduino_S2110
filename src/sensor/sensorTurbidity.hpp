#ifndef _SENSOR_TURBIDITY_H
#define _SENSOR_TURBIDITY_H

#include "sensorClass.hpp"

class sensorTurbidity : public sensorClass
{
public:
    sensorTurbidity() : sensorClass("Turbidity"){};
    ~sensorTurbidity(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        TURBIDITY = 0,
        MAX
    };
};

uint16_t sensorTurbidity::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorTurbidity::MAX; i++)
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

bool sensorTurbidity::sample()
{
    GROVE_SWITCH_ADC;

    float value = analogRead(SENSOR_ANALOG_PIN) * (5.0 / 1024.0);

    m_valueVector[sensorTurbidity::TURBIDITY].value.s32 = (int)(value * SCALE);

    return true;
}

bool sensorTurbidity::connected()
{
    return _connected;
}

#endif