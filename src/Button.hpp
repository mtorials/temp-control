#pragma once

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

#include "Display.hpp"
#include "Point.hpp"

#define LABEL_SIZE 0.9

typedef void OnClick(Waveshare_ILI9486 * tft, TemperatureCore * core);

class Button {
private:
  Waveshare_ILI9486 * tft;
  TemperatureCore * core;
  OnClick * callback = {};
  Point size = {0, 0};
  Point pos = {0, 0};
  uint16_t color;
  char * label;
public:
  Button(Waveshare_ILI9486 * Tft, TemperatureCore * core, Point pos, Point size, OnClick onClick, uint16_t color = 0xAAAA, char * label = "") {
    this->tft = Tft;
    this->callback = onClick;
    this->size = size;
    this->pos = pos;
    this->color = color;
    this->label = label;
  }
  Button() {}
  bool checkIfPointInButton(Point point) {
    bool xCheck = point.x > pos.x && point.x < pos.x + size.x;
    bool yCheck = point.y > pos.y && point.y < pos.y + size.y;
    return yCheck && xCheck;
  }
  void drawButton() {
    tft->fillRect(pos.x, pos.y, size.x, size.y, color);
    tft->setCursor(pos.x, pos.y + pos.y / size.y);
    tft->setTextSize(LABEL_SIZE);
    tft->print(label);
  }
  void execute() {
    this->callback(tft, core);
  }
};