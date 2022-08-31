#include "Display.hpp"
#include "Colors.hpp"
#include "defs.hpp"
#include "EditStatus.hpp"

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

// Default log_data -> nothing active
void Display::drawTempCurve(TemperatureCore *core, EditableValues activeValue = LOG_DATA)
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
      // Draw as active or not
      if (activeValue == Tmp0) activeValue = T1;
      if (activeValue == Tmp1) activeValue = T2;
      if (activeValue == Tmp2) activeValue = T3;
      if (!core->getCurvePart(t) == activeValue) Tft.fillRect(x, y, pointSize, pointSize, GRAPH_COLOR);
      else Tft.fillRect(x, y, pointSize, pointSize, GRAPH_ACTIVE_COLOR);
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
  if (p.z < TOUCH_THRESHOLD)
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
  drawTempCurve(this->core, this->controlUI->currentlyEditing->getValue());
  drawButtons();
}

void Display::drawTemperatures()
{
  int offset = 10;
  int spacing = 100;

  // clear area
  Tft.fillRect(0, 0, Tft.LCD_WIDTH, 60 + offset, BG_COLOR);

  Tft.setCursor(offset, offset);

  EditableValues active = this->controlUI->currentlyEditing->getValue();
  Tft.print(this->getTemperatureCore()->getValueForPart(active));
  if (active == Tmp0 || active == Tmp1 || active == Tmp2) Tft.print(" C");
  else Tft.print(" min");
  
  ////////////
  // Bottom
  ////////////

  const int bigY = 400;
  // clear area
  Tft.fillRect(offset, bigY, 150, 40, BLACK);
  // print status big
  Tft.setCursor(offset, bigY);
  Tft.setTextSize(2);
  Tft.print("T:     ");
  Tft.print(this->status->currentTempereature);
  Tft.print(" C");
  Tft.setCursor(offset, bigY + 20);
  Tft.print("T_SET: ");
  Tft.print(getTemperatureCore()->getTemperature());
  Tft.print(" C");

  Tft.setTextSize(2);

  // clear bottom text
  Tft.fillRect(0, Tft.LCD_HEIGHT - 20, Tft.LCD_WIDTH, 20, BG_COLOR);
  // draw bottom text
  Tft.setCursor(offset, Tft.LCD_HEIGHT - 20);
  if (this->core->running())
  {
    Tft.print("Running. Remaining ");
    Tft.print(this->core->getRemainingTime());
    Tft.print("min");
  }
  else
  {
    Tft.print("Ready.");
  }
}

// void Display::drawTemperatures()
// {
//   int offset = 10;
//   int spacing = 100;

//   // clear area
//   Tft.fillRect(0, 0, Tft.LCD_WIDTH, 60 + offset, BG_COLOR);

//   // draw temps
//   Tft.setCursor(offset, offset);
//   Tft.setTextSize(1);
//   Tft.print("Tmp: ");
//   Tft.print(this->status->currentTempereature);
//   Tft.print("C");
//   Tft.setCursor(offset + spacing, offset);
//   Tft.print("T_set: ");
//   Tft.print(this->core->getTemperature());
//   Tft.print("C");
//   Tft.setCursor(offset + 2 * spacing, offset);
//   if (status->heating)
//   {
//     Tft.print("Heating...");
//   }
//   else
//   {
//     Tft.print("-");
//   }

//   Tft.setCursor(offset, offset + 10);
//   Tft.print("t: ");
//   Tft.print(this->core->getTime());
//   Tft.print("min");
//   Tft.setCursor(offset + spacing, offset + 10);
//   Tft.print("Duration: ");
//   Tft.print(this->core->getDuration());
//   Tft.print("min");

//   Tft.setCursor(offset, offset + 20);
//   Tft.print("Tmp_0: ");
//   Tft.print(this->core->getTemperatureCurve()->tmp0);
//   Tft.print("C");
//   Tft.setCursor(offset + spacing, offset + 20);
//   Tft.print("Tmp_1: ");
//   Tft.print(this->core->getTemperatureCurve()->tmp1);
//   Tft.print("C");
//   Tft.setCursor(offset + 2 * spacing, offset + 20);
//   Tft.print("Tmp_2: ");
//   Tft.print(this->core->getTemperatureCurve()->tmp2);
//   Tft.print("C");

//   Tft.setCursor(offset, offset + 30);
//   Tft.print("t_1: ");
//   Tft.print(this->core->getTemperatureCurve()->t1);
//   Tft.print("min");
//   Tft.setCursor(offset + spacing, offset + 30);
//   Tft.print("t_2: ");
//   Tft.print(this->core->getTemperatureCurve()->t2);
//   Tft.print("min");
//   Tft.setCursor(offset + 2 * spacing, offset + 30);
//   Tft.print("t_3: ");
//   Tft.print(this->core->getTemperatureCurve()->t3);
//   Tft.print("min");

//   Tft.setCursor(offset, offset + 40);
//   Tft.print("rmp_1: ");
//   Tft.print(this->core->getTemperatureCurve()->rmp1);
//   Tft.print("min");
//   Tft.setCursor(offset + spacing, offset + 40);
//   Tft.print("rmp_2: ");
//   Tft.print(this->core->getTemperatureCurve()->rmp2);
//   Tft.print("min");
//   Tft.setCursor(offset + spacing * 2, offset + 40);
//   Tft.print("rmp_3: ");
//   Tft.print(this->core->getTemperatureCurve()->rmp3);
//   Tft.print("min");

//   const int bigY = 400;
//   // clear area
//   Tft.fillRect(offset, bigY, 150, 40, BLACK);
//   // print status big
//   Tft.setCursor(offset, bigY);
//   Tft.setTextSize(2);
//   Tft.print("T:     ");
//   Tft.print(this->status->currentTempereature);
//   Tft.print(" C");
//   Tft.setCursor(offset, bigY + 20);
//   Tft.print("T_SET: ");
//   Tft.print(getTemperatureCore()->getTemperature());
//   Tft.print(" C");

//   Tft.setTextSize(2);

//   // clear bottom text
//   Tft.fillRect(0, Tft.LCD_HEIGHT - 20, Tft.LCD_WIDTH, 20, BG_COLOR);
//   // draw bottom text
//   Tft.setCursor(offset, Tft.LCD_HEIGHT - 20);
//   if (this->core->running())
//   {
//     Tft.print("Running. Remaining ");
//     Tft.print(this->core->getRemainingTime());
//     Tft.print("min");
//   }
//   else
//   {
//     Tft.print("Ready.");
//   }
// }