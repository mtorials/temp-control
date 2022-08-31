#pragma once

#include "TemperatureCurve.hpp"
#include "DataLogger.hpp"
#include "EditStatus.hpp"

class TemperatureCore
{
private:
  TemperatureCurve *curve = nullptr;
  int systemT = 0;
  bool started = false;
  int startedAt = 0;
  DataLogger *dataLogger = nullptr;

private:
  int getTmpForRmp(int from, int to, int in, int starting, int t)
  {
    float m = (to - from) / in;
    return m * (t - starting) + from;
  }

public:
  TemperatureCore(TemperatureCurve *curve)
  {
    this->curve = curve;
    this->dataLogger = new DataLogger(getDuration());
  }

  DataLogger *getDataLogger()
  {
    return this->dataLogger;
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
  EditableValues getCurvePart(int t);
  int getValueForPart(EditableValues part);

  void setTime(int t);
  int getTime();

  void start();
  void stop();

  int getTemperature();
  int getTemperatureForT(int t);
  int getDuration();
  int getRemainingTime();
};