#include <Arduino.h>
#include "sensorBuilder.hpp"

SensorBuilderClass SensorBuilder;

void setup()
{
  Serial.begin(9600);
  // while (!Serial)
  //   ;

  SensorBuilder.check_grove();

  Serial.println("SensorBuilder add CO2");
  sensorCO2 *co2 = new sensorCO2();
  SensorBuilder.addSensor(co2);

  Serial.println("SensorBuilder add Light");
  sensorLight *light = new sensorLight();
  SensorBuilder.addSensor(light);

  Serial.println("SensorBuilder add Flame");
  sensorFlame *flame = new sensorFlame();
  SensorBuilder.addSensor(flame);

  Serial.println("SensorBuilder add O2");
  sensorO2 *o2 = new sensorO2();
  SensorBuilder.addSensor(o2);

  Serial.println("SensorBuilder add Sunlight");
  sensorSunlight *sunlight = new sensorSunlight();
  SensorBuilder.addSensor(sunlight);

  Serial.println("SensorBuilder add BMP280");
  sensorBMP280 *bmp280 = new sensorBMP280();
  SensorBuilder.addSensor(bmp280);

  Serial.println("SensorBuilder add BME688");
  sensorBME688 *bme688 = new sensorBME688();
  SensorBuilder.addSensor(bme688);

  Serial.println("SensorBuilder add MultiGas");
  sensorMultiGas *gas = new sensorMultiGas();
  SensorBuilder.addSensor(gas);

  Serial.println("SensorBuilder add UV");
  sensorUV *uv = new sensorUV();
  SensorBuilder.addSensor(uv);

  Serial.println("SensorBuilder add Turbidity");
  sensorTurbidity *turbidity = new sensorTurbidity();
  SensorBuilder.addSensor(turbidity);

  Serial.println("SensorBuilder add TDS");
  sensorTDS *tds = new sensorTDS();
  SensorBuilder.addSensor(tds);

  Serial.println("SensorBuilder add Ultrasonic");
  sensorUltrasonic *ultrasonic = new sensorUltrasonic();
  SensorBuilder.addSensor(ultrasonic);

  SensorBuilder.begin();
}

void loop()
{
  SensorBuilder.poll();
}
