#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <TemperatureCurve.hpp>
#include "TemperatureCore.hpp"
#include "Status.hpp"
#include "Button.hpp"
#include "Point.hpp"

#define BUTTON_COUNT 2

class ControlUI {
private:
  Button buttons[BUTTON_COUNT];
  Selection selection = { 0 };
  char * editing = "-";
public:
  Button * getButtons() {
    return buttons;
  }
  ControlUI() {}
  ControlUI(Waveshare_ILI9486 * tft, TemperatureCore * core) {
    Point size = {50, 50};
    Point start = {100, 300};
    Point pos = start;
    short offset = 5;

    // Start/Stop
    buttons[0] = Button(tft, {core, &selection}, pos, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      ctx->core->start();
    }, BLUE, "Start");
    pos = {(short)(start.x + size.x + offset), start.y};
    buttons[1] = Button(tft, {core, &selection}, pos, size, [](Waveshare_ILI9486 * tft, ButtonContext * ctx) {
      ctx->core->stop();
    }, BLUE, "Stop");
  }
};