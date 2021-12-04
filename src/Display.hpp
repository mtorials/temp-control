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

#define TOUCH_THRESHOLD 520

namespace
{
  Waveshare_ILI9486 Tft;
}

class Display
{
private:
  TemperatureCore * core;
  Status * status;
  ControlUI controlUI;
  TSPoint getTouchPoint() {
    TSPoint p = Tft.getPoint();
    Tft.normalizeTsPoint(p);
    return p;
  }
  void drawTempCurve(TemperatureCore* core, Point start = {0, 0}, Point end = {200, 100}) {
    TemperatureCurve * curve = core->getTemperatureCurve();
    int tMax = curve->t1 + curve->t2 + curve->t3 + curve->rmp1 + curve->rmp2 + curve->rmp3;
    for (int t = 0; t < tMax; t++) {
      int x = ((float)t / (float)tMax) * (end.x - start.x);
      int y = ((float)core->getTemperatureForT(t) / (float)core->getMaxTemp()) * (end.y - start.y);
      // flip y axis
      y = end.y - y;
      Tft.drawRect(x, y, 4, 4, 0xFFFF);
    }
  }
  void drawTemperatures() {
    int offset = 10;
    Tft.setCursor(offset, offset);
    Tft.setTextSize(1);
    Tft.print("T: ");
    Tft.print(this->status->currentTempereature);
    Tft.print("C");
    Tft.setCursor(offset + 100, offset);
    Tft.print("T_soll: ");
    Tft.print(core->getTemperature());
    Tft.print("C");
    Tft.println();
    Tft.print("t = ");
    Tft.print(core->getTime());
  }
  void registerButtons() {

  }
  void drawButtons() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
      controlUI.getButtons()[i].drawButton();
    }
  }
public:
  Display(TemperatureCore * core, Status * status) {
    this->core = core;
    this->status = status;
    this->controlUI = ControlUI(&Tft, core);
  }
  Waveshare_ILI9486 * getTft() {
    return &Tft;
  }
  TemperatureCore * getTemperatureCore() {
    return core;
 }
  void begin()
  {
    Tft.begin();
    Tft.setRotation(0);
    Tft.setTextSize(2);
    drawTempCurve(core);
    registerButtons();
    drawButtons();
  }
  void loop() {
    TSPoint p = getTouchPoint();
    //Serial.println(p.z);
    if (p.z < 580) return;
    for (int i = 0; i < BUTTON_COUNT; i++) {
      if (controlUI.getButtons()[i].checkIfPointInButton({p.x, p.y})) {
        controlUI.getButtons()[i].execute();
        Tft.fillScreen(0xFFFF);
        update();
        break;
      }
    }
  }
  void update() {
    Tft.fillScreen(BLACK);
    drawTemperatures();
    drawTempCurve(core);
    drawButtons();
  }
};