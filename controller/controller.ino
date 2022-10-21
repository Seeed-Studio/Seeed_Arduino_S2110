#include <Arduino.h>
#include "controller.hpp"

controllerClass controller;

void setup()
{
  Serial.begin(9600);

  controller.begin();
}

void loop()
{
  controller.poll();
}
