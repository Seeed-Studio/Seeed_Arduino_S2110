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



## Software Set Up

- **Step 1:** Install the necessary sensor support library :

   [Grove_BMP280](https://github.com/Seeed-Studio/Grove_BMP280)  
   [Grove_Sunlight_Sensor](https://github.com/Seeed-Studio/Grove_Sunlight_Sensor/tree/Si1151)  
   [Seeed_Arduino_MultiGas](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas)  
   [Seeed_Arduino_BME68x](https://github.com/Seeed-Studio/Seeed_Arduino_BME68x)  
   [Seeed_Arduino_UltrasonicRanger](https://github.com/Seeed-Studio/Seeed_Arduino_UltrasonicRanger)  
   [Seeed_Arduino_MultiGas](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas)  
   [arduino-i2c-scd4x](https://github.com/Sensirion/arduino-i2c-scd4x)  
   [ArduinoRS485](https://github.com/arduino-libraries/ArduinoRS485)  
   [ArduinoModbus](https://github.com/arduino-libraries/ArduinoModbus)
   
- **Step 2:** Install `Seeed Studio XIAO RP2040` board. 

 refer to [Seeed Studio XIAO RP2040 with Arduino | Seeed Studio Wiki](https://wiki.seeedstudio.com/XIAO-RP2040-with-Arduino)

- **Step 2:** Add pin defines.

 Add the following lines at the end of the `xx\Arduino15\packages\rp2040\hardware\rp2040\3.1.0\variants\seeed_xiao_rp2040\pins_arduino.h` file.
```c
// RS485
#define RS485_SERIAL_PORT Serial1
#define RS485_DEFAULT_DE_PIN D8
#define RS485_DEFAULT_RE_PIN D8
#define RS485_DEFAULT_TX_PIN 1
```



## How to Add a New Sensor

- **Step 1:** Add a `sensorNew.hpp` file to the `src\sensor` folder for the new sensor.

- **Step 2:** Define the sensor class and implement the `init()` and `sample()` functions.

 The sensor class should inherits from the `sensorClass` class, and implement the `init()` and `sample()` functions. 
   The `init()` function is used to initialize the sensor, and than returns a register offset value for Modbus communication.
   The `sample()` function is used to read the sensor data, returns true when the data is valid, and returns false when the data is invalid.

- **Step 3:** Include the `sensorNEW.hpp` file and call it.

 Add line `#include "sensorNew.hpp"` to the `src\sensor\sensorBuilder.hpp` file.
 In the `setup()` function of the `sensorBuilder.ino` file, create the new sensor class object and call the `SensorBuilder.addSensor()` function with it as an argument. 

Refer to the following code :

```c++
void setup()
{
  Serial.begin(9600);
  SensorBuilder.check_grove();
 
  /* sensor list */
  sensorUltrasonic *ultrasonic = new sensorUltrasonic();
  SensorBuilder.addSensor(ultrasonic);
  
  // add new sensor to sensor list
  sensorNew *newSensor = new sensorNew();
  SensorBuilder.addSensor(newSensor);

  SensorBuilder.begin();
}
```

> NOTE: the Modbus register address for the new sensor will start from `0x0034`, the register bit width for each measurement value is 32, so the register address offset between two adjacent measurement values is 2.



## Modbus Register Table

<table>
<thead>
  <tr>
    <th>Grove Sensor Name</th>
    <th>Register Name</th>
    <th>Register Address<br />(Hexadecimal)</th>
    <th>Register Address<br />(Decimal)</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="3">Grove - CO2 &amp; Temperature &amp; Humidity Sensor (SCD41)</td>
    <td>Temperature</td>
    <td>0x0004</td>
    <td>04</td>
  </tr>
  <tr>
    <td>Humidity</td>
    <td>0x0006</td>
    <td>06</td>
  </tr>
  <tr>
    <td>CO2</td>
    <td>0x0008</td>
    <td>08</td>
  </tr>
  <tr>
    <td>Grove - Light Sensor v1.2</td>
    <td>Light</td>
    <td>0x000A</td>
    <td>10</td>
  <tr>
    <td>Grove - Flame Sensor</td>
    <td>Flame</td>
    <td>0x000C</td>
    <td>12</td>
  </tr>
  <tr>
    <td>Grove - Oxygen Sensor (MIX8410)</td>
    <td>Oxygen</td>
    <td>0x000E</td>
    <td>14</td>
  </tr>
  <tr>
    <td rowspan="3">Grove - Sunlight sensor (SI1151)</td>
    <td>Light Intensity</td>
    <td>0x0010</td>
    <td>16</td>
  </tr>
  <tr>
    <td>Visible Light</td>
    <td>0x0012</td>
    <td>18</td>
  </tr>
  <tr>
    <td>UV</td>
    <td>0x0014</td>
    <td>20</td>
  </tr>
  <tr>
    <td rowspan="3">Grove Temperature and Barometer Sensor (BMP280)</td>
    <td>Barometric Temperature</td>
    <td>0x0016</td>
    <td>22</td>
  </tr>
  <tr>
    <td>Atmospheric Pressure</td>
    <td>0x0018</td>
    <td>24</td>
  </tr>
  <tr>
    <td>Height</td>
    <td>0x001A</td>
    <td>26</td>
  </tr>
  <tr>
     <td rowspan="4">Grove - Temperature Humidity Pressure Gas Sensor(BME680)</td>
     <td>Temperature</td>
     <td>0x001C</td>
     <td>28</td>
   </tr>
     <tr>
    <td>Atmospheric Pressure</td>
    <td>0x001E</td>
    <td>30</td>
  </tr>
    <tr>
    <td>Humidity</td>
    <td>0x0020</td>
    <td>32</td>
  </tr>
    <tr>
    <td>Air Quality(VOC)</td>
    <td>0x0022</td>
    <td>34</td>
  </tr>
  <tr>
     <td rowspan="4">Grove - Gas Sensor V2(Multichannel)</td>
     <td>N02</td>
     <td>0x0024</td>
     <td>36</td>
   </tr>
     <tr>
    <td>C2H50H</td>
    <td>0x0026</td>
    <td>38</td>
  </tr>
    <tr>
    <td>VOC</td>
    <td>0x0028</td>
    <td>40</td>
  </tr>
    <tr>
    <td>CO</td>
    <td>0x002A</td>
    <td>42</td>
  <tr>
    <td>Grove - UV Sensor</td>
    <td>UV Intensity</td>
    <td>0x002C</td>
    <td>44</td>
  </tr>
    <tr>
    <td>Grove - Turbidity Sensor Meter V1.0</td>
    <td>Turbidity</td>
    <td>0x002E</td>
    <td>46</td>
  </tr>
    <tr>
    <td>Grove - TDS Sensor</td>
    <td>TDS</td>
    <td>0x0030</td>
    <td>48</td>
  </tr>
    <tr>
    <td>Grove - Ultrasonic Ranger</td>
    <td>Distance</td>
    <td>0x0032</td>
    <td>50</td>
  </tr>
</tbody>
</table>
> Modbus register addresses 0x0000 to 0x0003 are reserved for storing module system information, where 0x0000 is the modbus address with a default value of 1 and a maximum value of 247, 0x0001 is the serial port baud rate with a default value of 96 (corresponding to 9600), and 0x0002 to 0x0003 are for software version.

