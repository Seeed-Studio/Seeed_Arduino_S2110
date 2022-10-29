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

    enum
    {
        SUNLIGHT_IR = 0x00,
        SUNLIGHT_VLR = 0x01,
        SUNLIGHT_UV = 0x02,
        MAX
    };

private:
    Si115X _si1151; // IIC
};

bool sensorSunlight::init()
{
    for (uint16_t i = 0; i < sensorSunlight::MAX; i++)
    {
        sensorClass::reg_t value;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.push_back(value);
    }

    GROVE_SWITCH_IIC;

    if (!_si1151.Begin())
    {
        _connected = false;
        return false;
    }

    _connected = true;

    return true;
}

bool sensorSunlight::sample()
{
    GROVE_SWITCH_IIC;

    float IR = _si1151.ReadHalfWord();
    float VLR = _si1151.ReadHalfWord_VISIBLE();
    float UV = _si1151.ReadHalfWord_UV();

    m_valueVector[SUNLIGHT_IR].value.s32 = IR * SCALE;
    m_valueVector[SUNLIGHT_VLR].value.s32 = VLR * SCALE;
    m_valueVector[SUNLIGHT_UV].value.s32 = UV * SCALE;

    return true;
}

bool sensorSunlight::connected()
{
    return _connected;
}

#endif