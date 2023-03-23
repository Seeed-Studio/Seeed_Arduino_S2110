#ifndef _SENSOR_CLASS_H
#define _SENSOR_CLASS_H

#include "Arduino.h"
#include <vector>

#define GROVE_SWITCH_PIN D10
#define SENSOR_ANALOG_PIN A3
#define SENSOR_ANALOG_E_PIN A2
#define SENSOR_DIGITAL_PIN D3
#define SENSOR_DIGITAL_E_PIN D2
#define SENSOR_IIC_SCL_PIN SCL
#define SENSOR_IIC_SDA_PIN SDA

#define GROVE_SWITCH_IIC                 \
    pinMode(GROVE_SWITCH_PIN, OUTPUT);   \
    digitalWrite(GROVE_SWITCH_PIN, LOW); \
    delay(10)
#define GROVE_SWITCH_ADC                  \
    pinMode(GROVE_SWITCH_PIN, OUTPUT);    \
    digitalWrite(GROVE_SWITCH_PIN, HIGH); \
    delay(10)

#define SCALE 1000

class sensorClass
{
public:
    sensorClass(const char *name);
    ~sensorClass();

    typedef enum
    {
        REG_TYPE_U16_AB = 0,
        REG_TYPE_S16_AB,
        REG_TYPE_U32_ABCD,
        REG_TYPE_U32_CDAB,
        REG_TYPE_S32_ABCD,
        REG_TYPE_S32_CDAB,
    } regType_t;

    typedef struct
    {
        uint16_t addr;
        regType_t type;
        union
        {
            uint16_t u16;
            int16_t s16;
            uint32_t u32;
            int32_t s32;
            float f32;
            uint8_t data[4];
        } value;
    } reg_t;

    virtual uint16_t init(uint16_t reg, bool i2c_available) = 0;
    virtual bool connected() = 0;
    virtual bool sample() = 0;
    std::vector<sensorClass::reg_t> getMeasureValue();
    uint16_t length();
    String name();

    static uint8_t valueLength(sensorClass::regType_t type);

protected:
    std::vector<reg_t> m_valueVector;
    bool _connected;
    String _name;
};

sensorClass::sensorClass(const char *name)
{
    _name = String(name);
}

sensorClass::~sensorClass()
{
}

String sensorClass::name()
{
    return _name;
}

std::vector<sensorClass::reg_t> sensorClass::getMeasureValue()
{
    return m_valueVector;
}

uint16_t sensorClass::length()
{
    return m_valueVector.size();
}

uint8_t sensorClass::valueLength(sensorClass::regType_t type)
{
    uint8_t len = 0;
    switch (type)
    {
    case REG_TYPE_U16_AB:
    case REG_TYPE_S16_AB:
        len = 1;
        break;
    default:
        len = 2;
        break;
    }
    return len;
}

#endif