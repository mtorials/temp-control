#include "Display.hpp"
#include "Colors.hpp"

TSPoint Display::getTouchPoint()
{
  TSPoint p = Tft.getPoint();
  Tft.normalizeTsPoint(p);
  return p;
}

void Display::drawTempCurve(TemperatureCore *core)
{
  Point start = {0, 50};
  Point end = {Tft.LCD_WIDTH, 150};
  TemperatureCurve *curve = core->getTemperatureCurve();
  int tMax = curve->t1 + curve->t2 + curve->t3 + curve->rmp1 + curve->rmp2 + curve->rmp3;
  for (int t = 0; t < tMax; t++)
  {
    int x = ((float)t / (float)tMax) * (end.x - start.x);
    int y = ((float)core->getTemperatureForT(t) / (float)core->getMaxTemp()) * (end.y - start.y);
    // flip y axis
    y = end.y - y;
    Tft.drawRect(x, y, 4, 4, 0xFFFF);
  }
}

void Display::drawTemperatures()
{
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

  Tft.setCursor(offset, offset + 10);
  Tft.print("t = ");
  Tft.print(core->getTime());

  Tft.setCursor(offset, offset + 20);
  Tft.print("Tmp_0 ");
  Tft.print(core->getTemperatureCurve()->tmp0);
  Tft.print("      ");
  Tft.print("Tmp_1 ");
  Tft.print(core->getTemperatureCurve()->tmp1);
  Tft.print("      ");
  Tft.print("Tmp_2 ");
  Tft.print(core->getTemperatureCurve()->tmp2);
}

void Display::drawButtons()
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    controlUI->getButtons()[i].drawButton();
  }
}

Display::Display(TemperatureCore *core, Status *status)
{
  this->core = core;
  this->status = status;
  this->controlUI = new ControlUI(&Tft, core);
}

Waveshare_ILI9486 *Display::getTft()
{
  return &Tft;
}

TemperatureCore *Display::getTemperatureCore()
{
  return core;
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
  drawTempCurve(core);
  drawButtons();
}

void Display::loop()
{
  TSPoint p = getTouchPoint();
  //Serial.println(p.z);
  if (p.z < 580)
    return;
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    if (controlUI->getButtons()[i].checkIfPointInButton({p.x, p.y}))
    {
      controlUI->getButtons()[i].execute();
      //Tft.fillScreen(0xFFFF);
      update();
      break;
    }
  }
}

void Display::update()
{
  Tft.fillScreen(BLACK);
  drawTemperatures();
  drawTempCurve(core);
  drawButtons();
}