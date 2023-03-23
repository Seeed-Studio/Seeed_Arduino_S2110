#ifndef _SENSOR_FLAME_H
#define _SENSOR_FLAME_H

#include "sensorClass.hpp"

#define FLAME_DIGITAL_PIN D3

class sensorFlame : public sensorClass
{
public:
    sensorFlame() : sensorClass("Flame"){};
    ~sensorFlame(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        FLAME = 0,
        MAX
    };
};

uint16_t sensorFlame::init(uint16_t reg, bool i2c_available)
{

    uint16_t t_reg = reg;
    for (uint16_t i = 0; i < sensorFlame::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
        t_reg += sensorClass::valueLength(value.type);
    }

    _connected = true;
    return t_reg - reg;
}

bool sensorFlame::sample()
{
    GROVE_SWITCH_ADC;

    pinMode(FLAME_DIGITAL_PIN, INPUT);
    int value = digitalRead(FLAME_DIGITAL_PIN);

    m_valueVector[FLAME].value.s32 = value * SCALE;

    return true;
}

bool sensorFlame::connected()
{
    return _connected;
}

#endif