#include "Button.hpp"
#include "Colors.hpp"

Button::Button(
    Waveshare_ILI9486 *tft, ButtonContext *context, Point pos, Point size, OnClick onClick, char *label, uint16_t color, ActivityChecker activityChecker = [](ButtonContext *) -> bool
    { return false; })
{
  this->tft = tft;
  this->context = context;
  this->color = color;
  this->callback = onClick;
  this->ac = activityChecker;
  this->size = size;
  this->pos = pos;
  this->color = color;
  this->label = label;

  // TODO remove
  //Serial.print("addr in constructor"); Serial.println((long)currentlyEditing);
  //currentlyEditing->setValue(EditableValues::Rmp1); // working!
}

bool Button::checkIfPointInButton(Point point)
{
  bool xCheck = point.x > pos.x && point.x < pos.x + size.x;
  bool yCheck = point.y > pos.y && point.y < pos.y + size.y;
  return yCheck && xCheck;
}

void Button::drawButton()
{
  uint16_t c = color;
  if (ac(context))
    c = BTN_COLOR_ACTIVE;
  //redraw buttons only if colors (active state) changes
  if (c != colorCurr)
  {
    tft->fillRect(pos.x, pos.y, size.x, size.y, c);
    tft->setCursor(pos.x, pos.y + pos.y / size.y);
    tft->setTextSize(LABEL_SIZE);
    tft->print(label);
    colorCurr = c;
  }
}

void Button::execute()
{
  // TODO remove
  Serial.print("Execute... on ");
  Serial.println(label);
  Serial.print(context != nullptr);
  Serial.println(callback != nullptr);
  //editing->setValue(EditableValues::Rmp2); // half working?!? (ui working, output not)
  this->callback(tft, context);
}