#pragma once

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

#include "Point.hpp"
#include "ButtonContext.hpp"

#define LABEL_SIZE 0.9

#define BTN_COLOR 0xFF00
#define BTN_COLOR_ACTIVE 0xF000

class Button
{
private:
  Waveshare_ILI9486 *tft;
  ButtonContext *context;
  OnClick *callback = {};
  ActivityChecker *ac;
  Point size = {0, 0};
  Point pos = {0, 0};
  uint16_t color;
  char *label;

public:
  Button(
      Waveshare_ILI9486 *tft, ButtonContext *context, Point pos, Point size, OnClick onClick, char *label, ActivityChecker activityChecker = [](ButtonContext *) -> bool
      { return false; });

  bool checkIfPointInButton(Point point);
  void drawButton();
  void execute();
};