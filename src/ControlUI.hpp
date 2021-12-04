#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <TemperatureCurve.hpp>
#include "TemperatureCore.hpp"
#include "Status.hpp"
#include "Button.hpp"
#include "Point.hpp"

#define BUTTON_COUNT 7

class ControlUI {
private:
  Button buttons[BUTTON_COUNT];
  EditableValues currentlyEditing = EditableValues::Tmp0;
  char * editing = "-";
  ValueGetter * getValue = [](EditableValues type, TemperatureCurve * curve) -> int * {
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

    default:
      Serial.println("ERROR");
      break;
    }
  };
public:
  Button * getButtons() {
    return buttons;
  }
  ControlUI() {}
  ControlUI(Waveshare_ILI9486 * tft, TemperatureCore * core) {
    Point size = {50, 50};
    Point start = {50, 200};
    short posY = start.y;
    short posX = start.x;
    short offset = 5;

    ButtonContext context = {
      core,
      &currentlyEditing,
      getValue
    };

    // Selection
    posX = start.x;
    buttons[0] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      *(ctx->currentEdit) = EditableValues::Tmp0;
    }, "Tmp0", [](ButtonContext * ctx) -> bool {
      return (*ctx->currentEdit) == EditableValues::Tmp0;
    });
    posX += offset + size.x;
    buttons[1] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      *(ctx->currentEdit) = EditableValues::Tmp1;
    }, "Tmp1", [](ButtonContext * ctx) -> bool {
      return (*ctx->currentEdit) == EditableValues::Tmp1;
    });
    posX += offset + size.x;
    buttons[6] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      *(ctx->currentEdit) = EditableValues::Tmp2;
    }, "Tmp2", [](ButtonContext * ctx) -> bool {
      return (*ctx->currentEdit) == EditableValues::Tmp2;
    });

    // Add and Clear
    posY += size.y + offset;
    posX = start.x;
    buttons[2] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      *(ctx->changeValue(*(ctx->currentEdit), ctx->core->getTemperatureCurve())) += 100;
    }, "+100");
    posX += offset + size.x;
    buttons[3] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      Serial.println(*(ctx->currentEdit));
      *(ctx->changeValue(*(ctx->currentEdit), ctx->core->getTemperatureCurve())) = 0;
    }, "Clear");

    // Start/Stop
    posY += size.y + offset;
    posX = start.x;
    buttons[4] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      ctx->core->start();
    }, "Start");
    posX += offset + size.x;
    buttons[5] = Button(tft, context, {posX, posY}, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      ctx->core->stop();
    }, "Stop");
  }
  EditableValues * getCurrentlyEditing() {
    return &currentlyEditing;
  }
};