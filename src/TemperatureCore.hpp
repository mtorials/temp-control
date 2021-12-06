#pragma once

#include "TemperatureCurve.hpp"

class TemperatureCore
{
private:
  TemperatureCurve *curve;
  int systemT = 0;
  bool started = false;
  int startedAt = 0;
  int getTmpForRmp(int from, int to, int in, int starting, int t)
  {
    float m = (to - from) / in;
    return m * (t - starting) + from;
  }

public:
  TemperatureCore(TemperatureCurve *curve)
  {
    this->curve = curve;
  }

  TemperatureCurve *getTemperatureCurve()
  {
    return this->curve;
  }

  bool running()
  {
    return started;
  }

  int getMaxTemp();

  void setTime(int t);
  int getTime();

  void start();
  void stop();

  int getTemperature();
  int getTemperatureForT(int t);
  int getDuration();
  int getRemainingTime();
};