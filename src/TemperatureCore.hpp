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
  TemperatureCurve* getTemperatureCurve() {
    return this->curve;
  };
  int getMaxTemp() {
    if (curve->tmp0 > curve->tmp1 && curve->tmp0 > curve->tmp2) return curve->tmp0;
    else if (curve->tmp1 > curve->tmp2) return curve->tmp1;
    else return curve->tmp2;
  }
  TemperatureCore(TemperatureCurve *curve)
  {
    this->curve = curve;
  };
  void setTime(int t)
  {
    this->systemT = t;
  }
  void start()
  {
    if (this->started)
      return;
    this->startedAt = this->systemT;
    this->started = true;
  }
  int getTime() {
    return this->systemT - this->startedAt;
  }
  void stop()
  {
    this->started = false;
  }
  int getTemperature() {
    if (!this->started)
      return 0;
    int t = getTime();
    return getTemperatureForT(t);
  };
  int getTemperatureForT(int t)
  {
    TemperatureCurve * c = curve;
    if (t < c->t1)
    {
      return c->tmp0;
    }
    else if (t < c->rmp1 + c->t1)
    {
      return getTmpForRmp(c->tmp0, c->tmp1, c->rmp1, c->t1, t);
    }
    else if (t < c->t2 + c->rmp1 + c->t1)
    {
      return c->tmp1;
    }
    else if (t < c->rmp2 + c->t2 + c->rmp1 + c->t1)
    {
      return getTmpForRmp(c->tmp1, c->tmp2, c->rmp2, c->t2 + c->rmp1 + c->t1, t);
    }
    else if (t < c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1)
    {
      return c->tmp2;
    }
    else if (t < c->rmp3 + c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1)
    {
      return getTmpForRmp(c->tmp2, 0, c->rmp3, c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1, t);
    }
    else
    {
      return 0;
    }
  };
};