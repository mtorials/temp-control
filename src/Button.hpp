#pragma once

#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

#include "Display.hpp"
#include "Point.hpp"
#include "Selection.hpp"
#include "EditableValues.hpp"
#include "Colors.hpp"

#define LABEL_SIZE 0.9

#define BTN_COLOR 0xFF00
#define BTN_COLOR_ACTIVE 0xF000

typedef int * ValueGetter(EditableValues, TemperatureCurve *);

struct ButtonContext {
  TemperatureCore * core;
  EditableValues * currentEdit;
  ValueGetter * changeValue;
};

typedef bool ActivityChecker(ButtonContext*);
typedef void OnClick(Waveshare_ILI9486*, ButtonContext*);

class Button {
private:
  Waveshare_ILI9486 * tft;
  ButtonContext context;  
  OnClick * callback = {};
  ActivityChecker * ac;
  Point size = {0, 0};
  Point pos = {0, 0};
  uint16_t color;
  char * label;
public:
  Button(Waveshare_ILI9486 * tft, ButtonContext ctx, Point pos, Point size, OnClick onClick, char * label, ActivityChecker activityChecker = [](ButtonContext *) -> bool { return false; }) {
    this->tft = tft;
    this->context = ctx;
    this->callback = onClick;
    this->ac = activityChecker;
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
    uint16_t c = BTN_COLOR;
    if (ac(&context)) c = BTN_COLOR_ACTIVE;
    tft->fillRect(pos.x, pos.y, size.x, size.y, c);
    tft->setCursor(pos.x, pos.y + pos.y / size.y);
    tft->setTextSize(LABEL_SIZE);
    tft->print(label);
  }
  void execute() {
    this->callback(tft, &context);
  }
};