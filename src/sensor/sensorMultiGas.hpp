#ifndef _SENSOR_MULTIGAS_H
#define _SENSOR_MULTIGAS_H

#include "sensorClass.hpp"
#include <Multichannel_Gas_GMXXX.h>
#include <Wire.h>

#define SENSOR_MULTIGAS_I2C_ADDR 0x08

class sensorMultiGas : public sensorClass
{
public:
    sensorMultiGas() : sensorClass("MultiGas"){};
    ~sensorMultiGas(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        GAS_NO2 = 0x00, // unit : PPM
        GAS_C2H5OH = 0x01,
        GAS_VOC = 0x02,
        GAS_CO = 0x03,
        MAX
    };

private:
    GAS_GMXXX<TwoWire> gas;
};

uint16_t sensorMultiGas::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorMultiGas::MAX; i++)
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
    Wire.beginTransmission(SENSOR_MULTIGAS_I2C_ADDR);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }

    GROVE_SWITCH_IIC;
    gas.begin(Wire, SENSOR_MULTIGAS_I2C_ADDR);
    // gas.begin(Wire, 0x10); // use the hardware I2C
    // gas.changeGMXXXAddr();

    _connected = true;

    return t_reg - reg;
}

bool sensorMultiGas::sample()
{
    GROVE_SWITCH_IIC;

    int32_t val_NO2 = gas.measure_NO2();
    int32_t val_C2H5OH = gas.measure_C2H5OH();
    int32_t val_VOC = gas.measure_VOC();
    int32_t val_CO = gas.measure_CO();

    m_valueVector[GAS_NO2].value.s32 = val_NO2 * SCALE;
    m_valueVector[GAS_C2H5OH].value.s32 = val_C2H5OH * SCALE;
    m_valueVector[GAS_VOC].value.s32 = val_VOC * SCALE;
    m_valueVector[GAS_CO].value.s32 = val_CO * SCALE;

    return true;
}

bool sensorMultiGas::connected()
{
    return _connected;
}

#endif