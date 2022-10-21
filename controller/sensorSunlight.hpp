#ifndef _SENSOR_SUNLINGHT_H
#define _SENSOR_SUNLINGHT_H

#include "sensorClass.hpp"
#include <Si115X.h>
#include <Wire.h>

class sensorSunlight : public sensorClass
{
public:
    sensorSunlight() : sensorClass("Sunlight"){};
    ~sensorSunlight(){};
    virtual bool init();
    virtual bool connected();
    virtual bool sample();

private:
    Si115X _si1151; // IIC
    bool _connected;
};

bool sensorSunlight::init()
{
    GROVE_SWITCH_IIC;

    if (!_si1151.Begin())
    {
        _connected = false;
        return false;
    }

    m_measureValueMap[sensorClass::REG_SUNLIGHT_IR] = 0;
    m_measureValueMap[sensorClass::REG_SUNLIGHT_VLR] = 0;
    m_measureValueMap[sensorClass::REG_SUNLIGHT_UV] = 0;

    _connected = true;
    return true;
}

bool sensorSunlight::sample()
{
    GROVE_SWITCH_IIC;

    float IR = _si1151.ReadHalfWord();
    float VLR = _si1151.ReadHalfWord_VISIBLE();
    float UV = _si1151.ReadHalfWord_UV();

    m_measureValueMap[sensorClass::REG_SUNLIGHT_IR] = IR * SCALE;
    m_measureValueMap[sensorClass::REG_SUNLIGHT_VLR] = VLR * SCALE;
    m_measureValueMap[sensorClass::REG_SUNLIGHT_UV] = UV * SCALE;

    return true;
}

bool sensorSunlight::connected()
{
    return _connected;
}

#endif