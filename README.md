# Build Your Own LoRaWAN Sensor With SenseCAP S2110 XIAO LoRaWAN SensorBuilder

This is the source code for [SenseCAP S2110 XIAO LoRaWAN SensorBuilder](https://www.seeedstudio.com/SenseCAP-XIAO-LoRaWAN-Controller-p-5474.html) and it provides an open-source tool to build your own LoRaWAN sensors.

## Hardware Set Up

- **Step 1:** Unscrew the four screws on the SensorBuilder lid to open the lid and unscrew the PG connector (with a coin)

<div align=center><img width=1000 src="https://files.seeedstudio.com/wiki/SenseCAP-S2110/92.jpg"></div>

- **Step 2:** If you have already made the connection between the XIAO SensorBuilder and the Data Logger with the 4 wires, it is recommended to remove the red wire which connects to the 5V pin of the XIAO SensorBuilder

<div align=center><img width=1000 src="https://files.seeedstudio.com/wiki/SenseCAP-S2110/97.jpg"></div>

- **Step 3:** Connect one end of a USB Type-C cable to the XIAO SensorBuilder and the other to the PC

<div align=center><img width=1000 src="https://files.seeedstudio.com/wiki/SenseCAP-S2110/102.png"/></div>

Now you can open [Arduino IDE](https://www.arduino.cc/en/software) on the PC and start tinkering!



## Getting Started

1. Install the necessary sensor support library :
   [Grove_BMP280](https://github.com/Seeed-Studio/Grove_BMP280)
   [Seeed_BME680](https://github.com/Seeed-Studio/Seeed_BME680)
   [Grove_Sunlight_Sensor](https://github.com/Seeed-Studio/Grove_Sunlight_Sensor/tree/Si1151)
   [Seeed_Arduino_MultiGas](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas)
   [Seeed_Arduino_UltrasonicRanger](https://github.com/Seeed-Studio/Seeed_Arduino_UltrasonicRanger)
   [Seeed_Arduino_MultiGas](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas)
   [arduino-i2c-scd4x](https://github.com/Sensirion/arduino-i2c-scd4x)
   [ArduinoRS485](https://github.com/arduino-libraries/ArduinoRS485)
   [ArduinoModbus](https://github.com/arduino-libraries/ArduinoModbus)
2. Install `Seeed Studio XIAO RP2040` board. refer to [Seeed Studio XIAO RP2040 with Arduino | Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-RP2040-with-Arduino)
3. Add the following lines at the end of the `xx\Arduino15\packages\rp2040\hardware\rp2040\3.1.0\variants\seeed_xiao_rp2040\pins_arduino.h` file.

```c
// RS485
#define RS485_SERIAL_PORT Serial1
#define RS485_DEFAULT_DE_PIN D8
#define RS485_DEFAULT_RE_PIN D8
#define RS485_DEFAULT_TX_PIN 1
```

