#include <Arduino.h>
#include "controller.hpp"

controllerClass controller;

void setup()
{
  Serial.begin(9600);

  while (!Serial)
    ;

  delay(3000);

  // light
  sensorLight *light = new sensorLight();
  controller.addSensor(light);

  // flame
  sensorFlame *flame = new sensorFlame();
  controller.addSensor(flame);

  // O2
  sensorO2 *o2 = new sensorO2();
  controller.addSensor(o2);

  // CO2
  sensorCO2 *co2 = new sensorCO2();
  controller.addSensor(co2);

  // sunlight
  sensorSunlight *sunlight = new sensorSunlight();
  controller.addSensor(sunlight);

  // BMP280
  sensorBMP280 *bmp280 = new sensorBMP280();
  controller.addSensor(bmp280);

  controller.begin();
}

void loop()
{
  controller.poll();
}
