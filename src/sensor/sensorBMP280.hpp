#ifndef _SENSOR_BMP280_H
#define _SENSOR_BMP280_H

#include "sensorClass.hpp"
#include <Seeed_BMP280.h>
#include <Wire.h>

#define MSL 102009 // Mean Sea Level in Pa

class sensorBMP280 : public sensorClass
{
public:
    sensorBMP280() : sensorClass("BMP280"){};
    ~sensorBMP280(){};

    uint16_t init(uint16_t reg);
    bool connected();
    bool sample();

    enum
    {

        TEMPERATURE,
        PRESSURE,
        ALTITUDE,
        MAX
    };

private:
    BMP280 _bmp280; // IIC
};

uint16_t sensorBMP280::init(uint16_t reg)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorBMP280::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
        t_reg += sensorClass::valueLength(value.type);
    }

    if (!i2c_available) {
        _connected = false;
        return 0;
    }
    GROVE_SWITCH_IIC;
    Wire.begin();
    Wire.beginTransmission(BMP280_ADDRESS);
    if (Wire.endTransmission() != 0) {
        _connected = false;
        return 0;
    }
    _connected = _bmp280.init();

    return t_reg - reg;
}

bool sensorBMP280::sample()
{
    GROVE_SWITCH_IIC;

    float temp = _bmp280.getTemperature();
    uint32_t pressure = _bmp280.getPressure();
    float altitude = 0;

    if (pressure != 0)
    {
        altitude = _bmp280.calcAltitude(MSL, pressure, temp);
    }

    m_valueVector[TEMPERATURE].value.s32 = temp * SCALE;
    m_valueVector[PRESSURE].value.s32 = pressure * SCALE;
    m_valueVector[ALTITUDE].value.s32 = altitude * SCALE;

    return true;
}

bool sensorBMP280::connected()
{
    return _connected;
}

#endif