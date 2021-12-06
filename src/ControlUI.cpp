#include "ControlUI.hpp"

ControlUI::ControlUI(Waveshare_ILI9486 *tft, TemperatureCore *core)
{
  Point size = {50, 50};
  Point start = {10, 170};
  short posY = start.y;
  short posX = start.x;
  short offset = 5;

  Serial.print("Edit mem addr ");
  Serial.println((long)&currentlyEditing);
  Serial.println(currentlyEditing->getValue());

  this->buttons = (Button *)calloc(BUTTON_COUNT, sizeof(Button));

  this->context = new ButtonContext({
      core,
      currentlyEditing,
      valueGetter,
  });

  // Selection
  posX = start.x;
  buttons[0] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      {
        ctx->currentEdit->setValue(EditableValues::Tmp0);
      },
      "Tmp0",
      [](ButtonContext *ctx) -> bool
      {
        return ctx->currentEdit->getValue() == EditableValues::Tmp0;
      });
  posX += offset + size.x;
  buttons[1] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      {
        ctx->currentEdit->setValue(EditableValues::Tmp1);
      },
      "Tmp1",
      [](ButtonContext *ctx) -> bool
      {
        return ctx->currentEdit->getValue() == EditableValues::Tmp1;
      });
  posX += offset + size.x;
  buttons[2] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      {
        ctx->currentEdit->setValue(EditableValues::Tmp2);
      },
      "Tmp2",
      [](ButtonContext *ctx) -> bool
      {
        return ctx->currentEdit->getValue() == EditableValues::Tmp2;
      });

  posY += size.y + offset;
  posX = start.x;
  buttons[3] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::T1); },
      "T1", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::T1; });
  posX += offset + size.x;
  buttons[4] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::T2); },
      "T2", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::T2; });
  posX += offset + size.x;
  buttons[5] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::T3); },
      "T3", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::T3; });

  posY += size.y + offset;
  posX = start.x;
  buttons[6] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::Rmp1); },
      "Rmp1", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::Rmp1; });
  posX += offset + size.x;
  buttons[7] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::Rmp2); },
      "Rmp2", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::Rmp2; });
  posX += offset + size.x;
  buttons[8] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->currentEdit->setValue(EditableValues::Rmp3); },
      "Rmp3", [](ButtonContext *ctx) -> bool
      { return ctx->currentEdit->getValue() == EditableValues::Rmp3; });

  // Add and Clear
  posY += size.y + offset;
  posX = start.x;
  buttons[9] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      {
        *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) += 100;
      },
      "+100");
  posX += offset + size.x;
  buttons[10] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      {
        Serial.println("hello!");
        Serial.println(ctx->currentEdit->getValue());
        Serial.println((long)ctx->currentEdit);
        *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) = 0;
      },
      "Clear");

  //Start / Stop
  posY += size.y + offset;
  posX = start.x;
  buttons[11] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->core->start(); },
      "Start");
  posX += offset + size.x;
  buttons[12] = Button(
      tft, context, {posX, posY}, size,
      [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
      { ctx->core->stop(); },
      "Stop");
}