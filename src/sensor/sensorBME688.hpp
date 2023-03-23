#ifndef _SENSOR_BME688_H
#define _SENSOR_BME688_H

#include "sensorClass.hpp"
#include <seeed_bme680.h>
#include <Wire.h>

#define SENSOR_BME688_I2C_ADDR 0x76

class sensorBME688 : public sensorClass
{
public:
    sensorBME688(uint8_t addr = SENSOR_BME688_I2C_ADDR) : sensorClass("BME688")
    {
        bme688 = new Seeed_BME680((uint8_t)addr);
    };
    ~sensorBME688(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        TEMPERATURE = 0x00, // C
        PRESSURE = 0x01,    // KPa
        HUMIDITY = 0x02,    // %
        GAS = 0x03,         // Kohms
        MAX
    };

private:
    Seeed_BME680 *bme688;
};

uint16_t sensorBME688::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorBME688::MAX; i++)
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
    Wire.beginTransmission(SENSOR_BME688_I2C_ADDR);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }

    Serial.println("bme688 init");
    GROVE_SWITCH_IIC;
    if (!bme688->init())
    {
        Serial.println("bme688 init failed");
        _connected = false;
        return t_reg - reg;
    }

    _connected = true;
    return t_reg - reg;
}

bool sensorBME688::sample()
{
    GROVE_SWITCH_IIC;

    Serial.println("bme688 sample");
    bme688->read_sensor_data();
    float temperature = bme688->sensor_result_value.temperature;
    float pressure = bme688->sensor_result_value.pressure / 1000.0;
    float humidity = bme688->sensor_result_value.humidity;
    float gas = bme688->sensor_result_value.gas / 1000.0;

    m_valueVector[TEMPERATURE].value.s32 = temperature * SCALE;
    m_valueVector[PRESSURE].value.s32 = pressure * SCALE;
    m_valueVector[HUMIDITY].value.s32 = humidity * SCALE;
    m_valueVector[GAS].value.s32 = gas * SCALE;

    return true;
}

bool sensorBME688::connected()
{
    return _connected;
}

#endif