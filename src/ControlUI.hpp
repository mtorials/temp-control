#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include "TemperatureCurve.hpp"
#include "TemperatureCore.hpp"
#include "Status.hpp"
#include "Button.hpp"
#include "Point.hpp"
#include "EditStatus.hpp"

#define BUTTON_COUNT 13

class ControlUI
{
private:
  Button *buttons;
  ValueGetter *valueGetter = [](EditableValues type, TemperatureCurve *curve) -> int *
  {
    Serial.println(type);
    switch (type)
    {
    case Tmp0:
      return &(curve->tmp0);
      Serial.println("Tmp0");
      break;
    case Tmp1:
      return &(curve->tmp1);
      Serial.println("Tmp1");
      break;
    case Tmp2:
      return &(curve->tmp2);
      Serial.println("Tmp2");
      break;
    case T1:
      return &(curve->t1);
      Serial.println("t1");
      break;
    case T2:
      return &(curve->t2);
      Serial.println("t1");
      break;
    case T3:
      return &(curve->t3);
      Serial.println("t3");
      break;
    case Rmp1:
      return &(curve->rmp1);
      Serial.println("rmp1");
      break;
    case Rmp2:
      return &(curve->rmp2);
      Serial.println("rmp2");
      break;
    case Rmp3:
      return &(curve->rmp3);
      Serial.println("rmp3");
      break;

    default:
      Serial.println("ERROR");
      break;
    }
  };

  ButtonContext *context = nullptr;

public:
  ControlUI(Waveshare_ILI9486 *tft, TemperatureCore *core);

  Button *getButtons()
  {
    return buttons;
  }

  EditStatus *getEditStatus()
  {
    return currentlyEditing;
  }

public:
  EditStatus *currentlyEditing = new EditStatus(EditableValues::Tmp0);
};