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

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

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

uint16_t sensorSunlight::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorSunlight::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
        t_reg += sensorClass::valueLength(value.type);
    }

    if (!i2c_available)
    {
        _connected = false;
        return t_reg - reg;
    }
    GROVE_SWITCH_IIC;
    Wire.begin();
    Wire.beginTransmission(_si1151.DEVICE_ADDRESS);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }
    if (!_si1151.Begin())
    {
        _connected = false;
        return t_reg - reg;
    }

    _connected = true;

    return t_reg - reg;
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