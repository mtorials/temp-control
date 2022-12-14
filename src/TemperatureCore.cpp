#include "TemperatureCore.hpp"
#include "DataLogger.hpp"
#include "EditStatus.hpp"
#include <Arduino.h>

int TemperatureCore::getMaxTemp()
{
  if (curve->tmp0 > curve->tmp1 && curve->tmp0 > curve->tmp2)
    return curve->tmp0;
  else if (curve->tmp1 > curve->tmp2)
    return curve->tmp1;
  else
    return curve->tmp2;
}

int TemperatureCore::getTime()
{
  if (!started)
    return 0;
  return this->systemT - this->startedAt;
}

void TemperatureCore::setTime(int t)
{
  this->systemT = t;
}

void TemperatureCore::start()
{
  if (this->started)
    return;
  this->startedAt = this->systemT;
  this->started = true;
  this->getDataLogger()->reset(getDuration());
}

void TemperatureCore::stop()
{
  this->started = false;
}

int TemperatureCore::getTemperature()
{
  if (!this->started)
    return 0;
  int t = getTime();
  return getTemperatureForT(t);
}

int TemperatureCore::getTemperatureForT(int t)
{
  TemperatureCurve *c = curve;
  if (t < c->t1)
    return c->tmp0;
  else if (t < c->rmp1 + c->t1)
    return getTmpForRmp(c->tmp0, c->tmp1, c->rmp1, c->t1, t);
  else if (t < c->t2 + c->rmp1 + c->t1)
    return c->tmp1;
  else if (t < c->rmp2 + c->t2 + c->rmp1 + c->t1)
    return getTmpForRmp(c->tmp1, c->tmp2, c->rmp2, c->t2 + c->rmp1 + c->t1, t);
  else if (t < c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1)
    return c->tmp2;
  else if (t < c->rmp3 + c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1)
    return getTmpForRmp(c->tmp2, 0, c->rmp3, c->t3 + c->rmp2 + c->t2 + c->rmp1 + c->t1, t);
  else
    return 0;
}

int TemperatureCore::getDuration()
{
  return (curve->t1 + curve->t2 + curve->t3 + curve->rmp1 + curve->rmp2 + curve->rmp3);
}

int TemperatureCore::getRemainingTime()
{
  int r = getDuration() - getTime();
  if (r < 0)
  {
    return 0;
  }
  return r;
}

EditableValues TemperatureCore::getCurvePart(int t) {
  if (t < curve->t1) return EditableValues::T1;
  else if (t < curve->t1 + curve->rmp1) return EditableValues::Rmp1;
  else if (t < curve->t1 + curve->rmp1 + curve->t2) return EditableValues::T2;
  else if (t < curve->t1 + curve->rmp1 + curve->t2 + curve->rmp2) return EditableValues::Rmp2;
  else if (t < curve->t1 + curve->rmp1 + curve->t2 + curve->rmp2 + curve->t3) return EditableValues::T3;
  else if (t < curve->t1 + curve->rmp1 + curve->t2 + curve->rmp2 + curve->t3 + curve->rmp3) return EditableValues::Rmp3;
  else Serial.println("Cant get part fot t...");
}

int TemperatureCore::getValueForPart(EditableValues part) {
switch (part) {
    case Tmp0:
      return (curve->tmp0);
      break;
    case Tmp1:
      return (curve->tmp1);
      break;
    case Tmp2:
      return (curve->tmp2);
      break;
    case T1:
      return (curve->t1);
      break;
    case T2:
      return (curve->t2);
      break;
    case T3:
      return (curve->t3);
      break;
    case Rmp1:
      return (curve->rmp1);
      break;
    case Rmp2:
      return (curve->rmp2);
      break;
    case Rmp3:
      return (curve->rmp3);
      break;

    default:
      Serial.println("ERROR");
      break;
    }
}

