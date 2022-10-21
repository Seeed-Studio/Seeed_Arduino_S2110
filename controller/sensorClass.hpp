#ifndef _SENSOR_CLASS_H
#define _SENSOR_CLASS_H

#include "Arduino.h"
#include <map>

#define GROVE_SWITCH_PIN D10
#define GROVE_SWITCH_IIC digitalWrite(GROVE_SWITCH_PIN, LOW)
#define GROVE_SWITCH_ADC digitalWrite(GROVE_SWITCH_PIN, HIGH)

#define SCALE 1000

class sensorClass
{
public:
    sensorClass(const char *name);
    ~sensorClass();
    virtual bool init() = 0;
    virtual bool connected() = 0;
    virtual bool sample() = 0;
    std::map<uint16_t, int32_t> getMeasureValue();
    uint16_t length();
    String name();

    enum REGISTERS
    {
        REG_ADDR = 0x00,
        REG_BAUD = 0x01,
        REG_VERSION = 0x02,
        REG_CO2_TEMPE = 0x04,
        REG_CO2_HUMIDITY = 0x06,
        REG_CO2_CO2 = 0x08,
        REG_LIGHT = 0x0A,
        REG_FLAME = 0x0C,
        REG_O2 = 0x0E,
        REG_SUNLIGHT_IR = 0x10,
        REG_SUNLIGHT_VLR = 0x12,
        REG_SUNLIGHT_UV = 0x14,
        REG_BMP280_TEMP = 0x16,
        REG_BMP280_PRESSURE = 0x18,
        REG_BMP280_ALTITUDE = 0x1A,
        REG_MAX = 0x1C,
    };

protected:
    std::map<uint16_t, int32_t> m_measureValueMap;

private:
    String _name;
    bool _connected;
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

std::map<uint16_t, int32_t> sensorClass::getMeasureValue()
{
    return m_measureValueMap;
}

uint16_t sensorClass::length()
{
    return m_measureValueMap.size();
}

#endif