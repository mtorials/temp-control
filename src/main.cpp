#include <Arduino.h>
#include "TemperatureCurve.hpp"
#include "TemperatureCore.hpp"

TemperatureCurve curve;
TemperatureCore core = TemperatureCore(&curve);

void setup() {
  Serial.begin(155200);
}

void loop() {
  long seconds = millis() / 500;
  core.setTime(seconds);
  Serial.println(core.getTemperature());
  if (seconds > 20) core.start();
}