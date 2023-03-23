#ifndef _SENSOR_ULTRASONIC_H
#define _SENSOR_ULTRASONIC_H

#include "sensorClass.hpp"
#include "Ultrasonic.h"

class sensorUltrasonic : public sensorClass
{
public:
    sensorUltrasonic(uint8_t pin = SENSOR_DIGITAL_PIN) : sensorClass("Ultrasonic")
    {
        ultrasonic = new Ultrasonic(pin);
    };
    ~sensorUltrasonic(){};
    uint16_t init(uint16_t reg, bool i2c_available);
    virtual bool connected();
    virtual bool sample();

    enum
    {
        ULTRASONIC_CM = 0x00,
        MAX
    };

private:
    Ultrasonic *ultrasonic;
};

uint16_t sensorUltrasonic::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;
    for (uint16_t i = 0; i < sensorUltrasonic::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        t_reg += sensorClass::valueLength(value.type);
        m_valueVector.emplace_back(value);
    }

    _connected = i2c_available ? false : true;
    
    return t_reg - reg;
    ;
}

bool sensorUltrasonic::sample()
{
    GROVE_SWITCH_ADC;

    int value = (int)ultrasonic->MeasureInCentimeters();
    m_valueVector[ULTRASONIC_CM].value.s32 = value * SCALE;

    return true;
}

bool sensorUltrasonic::connected()
{
    return _connected;
}

#endif