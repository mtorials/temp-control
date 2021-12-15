#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>
#include <TemperatureCurve.hpp>
#include "TemperatureCore.hpp"
#include "Status.hpp"
#include "Point.hpp"
#include "Button.hpp"
#include "ControlUI.hpp"
#include "DataLogger.hpp"
#include "defs.hpp"

namespace
{
  Waveshare_ILI9486 Tft;
}

class Display
{
private:
  TemperatureCore *core;
  Status *status;
  ControlUI *controlUI;
  TSPoint getTouchPoint();
  void drawTempCurve(TemperatureCore *core);
  void drawTemperatures();
  void drawButtons();

public:
  Display(TemperatureCore *core, Status *status);
  Waveshare_ILI9486 *getTft();
  TemperatureCore *getTemperatureCore();
  ControlUI *getControlUI();
  void begin();
  void loop();
  void update();
};