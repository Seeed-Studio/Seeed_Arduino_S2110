#ifndef _SENSOR_FLAME_H
#define _SENSOR_FLAME_H

#include "sensorClass.hpp"

#define FLAME_DIGITAL_PIN D3

class sensorFlame : public sensorClass
{
public:
    sensorFlame() : sensorClass("Flame"){};
    ~sensorFlame(){};
    virtual bool init();
    virtual bool connected();
    virtual bool sample();

private:
    bool _connected;
};

bool sensorFlame::init()
{

    m_measureValueMap[sensorClass::REG_FLAME] = 0;

    _connected = true;
    return true;
}

bool sensorFlame::sample()
{
    GROVE_SWITCH_ADC;

    int value = digitalRead(FLAME_DIGITAL_PIN);

    m_measureValueMap[sensorClass::REG_FLAME] = value * SCALE;

    return true;
}

bool sensorFlame::connected()
{
    return _connected;
}

#endif