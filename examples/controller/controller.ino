#include <Arduino.h>
#include "controller.hpp"

controllerClass controller;

void setup()
{
  Serial.begin(9600);
  // while (!Serial)
  //   ;

  controller.check_grove();

  Serial.println("controller add CO2");
  sensorCO2 *co2 = new sensorCO2();
  controller.addSensor(co2);

  Serial.println("controller add Light");
  sensorLight *light = new sensorLight();
  controller.addSensor(light);

  Serial.println("controller add Flame");
  sensorFlame *flame = new sensorFlame();
  controller.addSensor(flame);

  Serial.println("controller add O2");
  sensorO2 *o2 = new sensorO2();
  controller.addSensor(o2);

  Serial.println("controller add Sunlight");
  sensorSunlight *sunlight = new sensorSunlight();
  controller.addSensor(sunlight);

  Serial.println("controller add BMP280");
  sensorBMP280 *bmp280 = new sensorBMP280();
  controller.addSensor(bmp280);

  Serial.println("controller add BME688");
  sensorBME688 *bme688 = new sensorBME688();
  controller.addSensor(bme688);

  Serial.println("controller add MultiGas");
  sensorMultiGas *gas = new sensorMultiGas();
  controller.addSensor(gas);

  Serial.println("controller add UV");
  sensorUV *uv = new sensorUV();
  controller.addSensor(uv);

  Serial.println("controller add Turbidity");
  sensorTurbidity *turbidity = new sensorTurbidity();
  controller.addSensor(turbidity);

  Serial.println("controller add TDS");
  sensorTDS *tds = new sensorTDS();
  controller.addSensor(tds);

  Serial.println("controller add Ultrasonic");
  sensorUltrasonic *ultrasonic = new sensorUltrasonic();
  controller.addSensor(ultrasonic);

  controller.begin();
}

void loop()
{
  controller.poll();
}
