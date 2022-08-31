#pragma once

#include <Arduino.h>
#include "Status.hpp"

#define UPPER_LIMIT 0
#define LOWER_LIMIT 20
#define CYCLE_LENGHT_IN_LOOPS 5000

typedef int *OnOffSetter(bool heat);

class Heater {
private:
  double percentage = 0;
  double t = 0;
public:
  void setHeatingTo(double percentage) {
    this->percentage = percentage;
  }
  void loop(Status * status) {
    if (t > CYCLE_LENGHT_IN_LOOPS) t = 0;
    double inCycle = t / (double)CYCLE_LENGHT_IN_LOOPS;
    if (inCycle < percentage) {
      status->heating = true;
    } else {
      status->heating = false;
    }
    this->t++;
  }
  void setHeating(int temp, int tSet) {
    int delta = temp - tSet;
    // 5 C too hot
    if (delta > (UPPER_LIMIT)) {
      percentage = 0;
    // 5 C too cold
    } else if (delta < -(LOWER_LIMIT)) {
      percentage = 100;
    } else {
      // linear funktion with f(delta) = percentage (0, 0.1) und (-20; 1)
      // I = [-20; 0]
      percentage = (-0.045) * delta + 0.1;
    }
    Serial.print("Heating set to: %");
    Serial.println(percentage);
  }
};