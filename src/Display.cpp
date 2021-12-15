#include "Display.hpp"
#include "Colors.hpp"
#include "defs.hpp"

Display::Display(TemperatureCore *core, Status *status)
{
  this->core = core;
  this->status = status;
  this->controlUI = new ControlUI(&Tft, core);
  Serial.println("Data Logger Display Constructor");
  Serial.println(this->core->getDataLogger() != nullptr);
}

TSPoint Display::getTouchPoint()
{
  TSPoint p = Tft.getPoint();
  Tft.normalizeTsPoint(p);
  return p;
}

void Display::drawTempCurve(TemperatureCore *core)
{
  Point start = {0, 70};
  Point end = {Tft.LCD_WIDTH, 185};
  short pointSize = 4;
  // clear area
  Tft.fillRect(start.x, start.y, Tft.LCD_WIDTH, end.y - start.y + pointSize, GRAPH_BG_COLOR);

  // draw graph
  TemperatureCurve *curve = this->core->getTemperatureCurve();
  int tMax = this->core->getDuration();

  // Draw Log graph
  if (controlUI->currentlyEditing->getValue() == EditableValues::LOG_DATA)
  {
    // TODO remove
    // Serial.print("Value hi of Datalogger: ");
    // Serial.println(this->core->getDataLogger()->hi);
    // Serial.print("[Display.cpp] Datalogger is null: ");
    // Serial.println(this->core->getDataLogger() == nullptr);
    // Serial.print("[Display.cpp] Datalogger->getValues() is null: ");
    // Serial.println(this->core->getDataLogger()->getValues() == nullptr);
    for (int t = 0; t < tMax; t++)
    {
      int x = ((float)t / (float)tMax) * (end.x - start.x);
      int y = ((float)this->core->getDataLogger()->getTempForT(t)) / ((float)(this->core->getMaxTemp() + LOG_GRAPH_MARGIN)) * (end.y - start.y);
      y = end.y - y;
      Tft.fillRect(x, y, pointSize, pointSize, GRAPH_LOG_COLOR);
    }
  }
  // Draw normal graph
  else
  {
    for (int t = 0; t < tMax; t++)
    {
      int x = ((float)t / (float)tMax) * (end.x - start.x);
      int y = ((float)this->core->getTemperatureForT(t) / (float)this->core->getMaxTemp()) * (end.y - start.y);
      // flip y axis
      y = end.y - y;
      Tft.fillRect(x, y, pointSize, pointSize, GRAPH_COLOR);
    }
  }

  // draw time indicator
  if (core->running())
  {
    int timeX = ((float)(this->core->getTime() + 1) / (float)tMax) * (end.x - start.x);
    int widthHalf = (int)((double)GRAPH_TIME_LINE_THICKNESS / 2.0);
    Tft.fillRect(timeX - widthHalf, start.y, GRAPH_TIME_LINE_THICKNESS, end.y - start.y, GRAPG_TIMELINE_COLOR);
  }
}

void Display::drawButtons()
{
  /*for (int i = 0; i < BUTTON_COUNT; i++)
  {
    controlUI->getButtons()[i].drawButton();
  }*/
  for (auto btn : controlUI->getButtons())
  {
    btn.drawButton();
  }
}

Waveshare_ILI9486 *Display::getTft()
{
  return &Tft;
}

TemperatureCore *Display::getTemperatureCore()
{
  return this->core;
}

ControlUI *Display::getControlUI()
{
  return controlUI;
}

void Display::begin()
{
  Tft.begin();
  Tft.setRotation(0);
  Tft.setTextSize(2);
  drawTempCurve(this->core);
  drawButtons();
}

void Display::loop()
{
  TSPoint p = getTouchPoint();
  // Serial.println(p.z);
  if (p.z < 580)
    return;
  for (auto btn : controlUI->getButtons())
  {
    if (btn.checkIfPointInButton({p.x, p.y}))
    {
      btn.execute();
      // Tft.fillScreen(0xFFFF);
      update();
      break;
    }
  }
}

void Display::update()
{
  // Tft.fillScreen(BG_COLOR);
  drawTemperatures();
  drawTempCurve(this->core);
  drawButtons();
}

void Display::drawTemperatures()
{
  int offset = 10;
  int spacing = 100;

  // clear area
  Tft.fillRect(0, 0, Tft.LCD_WIDTH, 60 + offset, BG_COLOR);

  // draw temps
  Tft.setCursor(offset, offset);
  Tft.setTextSize(1);
  Tft.print("Tmp: ");
  Tft.print(this->status->currentTempereature);
  Tft.print("C");
  Tft.setCursor(offset + spacing, offset);
  Tft.print("T_set: ");
  Tft.print(this->core->getTemperature());
  Tft.print("C");
  Tft.setCursor(offset + 2 * spacing, offset);
  if (status->heating)
  {
    Tft.print("Heating...");
  }
  else
  {
    Tft.print("-");
  }

  Tft.setCursor(offset, offset + 10);
  Tft.print("t: ");
  Tft.print(this->core->getTime());
  Tft.print("min");
  Tft.setCursor(offset + spacing, offset + 10);
  Tft.print("Duration: ");
  Tft.print(this->core->getDuration());
  Tft.print("min");

  Tft.setCursor(offset, offset + 20);
  Tft.print("Tmp_0: ");
  Tft.print(this->core->getTemperatureCurve()->tmp0);
  Tft.print("C");
  Tft.setCursor(offset + spacing, offset + 20);
  Tft.print("Tmp_1: ");
  Tft.print(this->core->getTemperatureCurve()->tmp1);
  Tft.print("C");
  Tft.setCursor(offset + 2 * spacing, offset + 20);
  Tft.print("Tmp_2: ");
  Tft.print(this->core->getTemperatureCurve()->tmp2);
  Tft.print("C");

  Tft.setCursor(offset, offset + 30);
  Tft.print("t_1: ");
  Tft.print(this->core->getTemperatureCurve()->t1);
  Tft.print("min");
  Tft.setCursor(offset + spacing, offset + 30);
  Tft.print("t_2: ");
  Tft.print(this->core->getTemperatureCurve()->t2);
  Tft.print("min");
  Tft.setCursor(offset + 2 * spacing, offset + 30);
  Tft.print("t_3: ");
  Tft.print(this->core->getTemperatureCurve()->t3);
  Tft.print("min");

  Tft.setCursor(offset, offset + 40);
  Tft.print("rmp_1: ");
  Tft.print(this->core->getTemperatureCurve()->rmp1);
  Tft.print("min");
  Tft.setCursor(offset + spacing, offset + 40);
  Tft.print("rmp_2: ");
  Tft.print(this->core->getTemperatureCurve()->rmp2);
  Tft.print("min");
  Tft.setCursor(offset + spacing * 2, offset + 40);
  Tft.print("rmp_3: ");
  Tft.print(this->core->getTemperatureCurve()->rmp3);
  Tft.print("min");

  // clear bottom text
  Tft.fillRect(0, Tft.LCD_HEIGHT - 20, Tft.LCD_WIDTH, 20, BG_COLOR);
  // draw bottom text
  Tft.setCursor(offset, Tft.LCD_HEIGHT - 20);
  if (this->core->running())
  {
    Tft.print("Running. Time remaining: ");
    Tft.print(this->core->getRemainingTime());
    Tft.print("min");
  }
  else
  {
    Tft.print("Ready.");
  }
}