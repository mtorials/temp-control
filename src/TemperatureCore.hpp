#include "TemperatureCurve.hpp"

class TemperatureCore
{
public:
  TemperatureCore(TemperatureCurve *curve) {
    this->curve = curve;
  };
  int getTemperature(int t) {
    TemperatureCurve *c = this->curve;
    if (t < c->t1) {
      return c->tmp0;
    }
    else if (t < c->rmp1 + c->t1) {
      return getTmpForRmp(c->tmp0, c->tmp1, c->rmp1, c->t1, t);
    }
    else if (t < c->t2 + c->rmp1 + c->t1) {
      return c->tmp1;
    }
    else if (t < c->rmp2 + c->t2 + c->rmp1 + c->t1) {
      return getTmpForRmp(c->tmp1, c->tmp2, c->rmp2, c->t2 + c->rmp1 + c->t1, t);
    }
    else if (t < c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1) {
      return c->tmp2;
    }
    else if (t < c->rmp3 + c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1) {
      return getTmpForRmp(c->tmp2, 0, c->rmp3, c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1, t);
    }
    else {
      return 0;
    }
  };
private:
  TemperatureCurve * curve;
  int getTmpForRmp(int from, int to, int in, int starting, int t) {
    float m = (to - from) / in;
    return m * (t - starting) + from;
  }
};