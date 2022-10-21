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
    virtual bool init();
    virtual bool connected();
    virtual bool sample();

private:
    BMP280 _bmp280; // IIC
    bool _connected;
};

bool sensorBMP280::init()
{
    GROVE_SWITCH_IIC;

    if (!_bmp280.init())
    {
        _connected = false;
        return false;
    }

    m_measureValueMap[sensorClass::REG_BMP280_TEMP] = 0;
    m_measureValueMap[sensorClass::REG_BMP280_PRESSURE] = 0;
    m_measureValueMap[sensorClass::REG_BMP280_ALTITUDE] = 0;

    _connected = true;
    return true;
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

    m_measureValueMap[sensorClass::REG_BMP280_TEMP] = temp * SCALE;
    m_measureValueMap[sensorClass::REG_BMP280_PRESSURE] = pressure * SCALE;
    m_measureValueMap[sensorClass::REG_BMP280_ALTITUDE] = altitude * SCALE;

    return true;
}

bool sensorBMP280::connected()
{
    return _connected;
}

#endif