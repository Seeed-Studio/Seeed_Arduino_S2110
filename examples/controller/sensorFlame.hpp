#ifndef _SENSOR_FLAME_H
#define _SENSOR_FLAME_H

#include "sensorClass.hpp"

#define FLAME_DIGITAL_PIN D3

class sensorFlame : public sensorClass
{
public:
    sensorFlame() : sensorClass("Flame"){};
    ~sensorFlame(){};

    bool init();
    bool connected();
    bool sample();

    enum
    {
        FLAME = 0,
        MAX
    };
};

bool sensorFlame::init()
{

    for (uint16_t i = 0; i < sensorFlame::MAX; i++)
    {
        sensorClass::reg_t value;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.push_back(value);
    }

    _connected = true;
    return true;
}

bool sensorFlame::sample()
{
    GROVE_SWITCH_ADC;

    int value = digitalRead(FLAME_DIGITAL_PIN);

    m_valueVector[FLAME].value.s32 = value * SCALE;

    return true;
}

bool sensorFlame::connected()
{
    return _connected;
}

#endif