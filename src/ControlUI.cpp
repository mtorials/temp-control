#include "ControlUI.hpp"

#include "Colors.hpp"

ControlUI::ControlUI(Waveshare_ILI9486 *tft, TemperatureCore *core)
{
  Point size = {50, 50};
  Point start = {25, 215};
  short posY = start.y;
  short posX = start.x;
  short offset = 5;

  Serial.print("Edit mem addr ");
  Serial.println((long)&currentlyEditing);
  Serial.println(currentlyEditing->getValue());

  //this->buttons = (Button *)calloc(BUTTON_COUNT, sizeof(Button));

  this->context = new ButtonContext({
      core,
      currentlyEditing,
      valueGetter,
  });

  // Selection
  posX = start.x;
  buttons.push_back(
      //buttons[0] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            ctx->currentEdit->setValue(EditableValues::Tmp0);
          },
          "Tmp_0", BLUE,
          [](ButtonContext *ctx) -> bool
          {
            return ctx->currentEdit->getValue() == EditableValues::Tmp0;
          }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[1] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            ctx->currentEdit->setValue(EditableValues::Tmp1);
          },
          "Tmp_1", BLUE,
          [](ButtonContext *ctx) -> bool
          {
            return ctx->currentEdit->getValue() == EditableValues::Tmp1;
          }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[2] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            ctx->currentEdit->setValue(EditableValues::Tmp2);
          },
          "Tmp_2", BLUE,
          [](ButtonContext *ctx) -> bool
          {
            return ctx->currentEdit->getValue() == EditableValues::Tmp2;
          }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[3] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::T1); },
          "t_1", BLUE_DARK, [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::T1; }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[4] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::T2); },
          "t_2", BLUE_DARK, [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::T2; }));

  posY += size.y + offset;
  posX = start.x;
  buttons.push_back(
      //buttons[5] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::T3); },
          "t_3", BLUE_DARK, [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::T3; }));

  posX += offset + size.x;
  buttons.push_back(
      //buttons[6] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::Rmp1); },
          "rmp_1", BLUE_LIGHT, [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::Rmp1; }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[7] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::Rmp2); },
          "rmp_2", BLUE_LIGHT, [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::Rmp2; }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[8] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->currentEdit->setValue(EditableValues::Rmp3); },
          "rmp_3", BLUE_LIGHT,
          [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::Rmp3; }));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[15] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            ctx->currentEdit->setValue(EditableValues::LOG_DATA);
          },
          "LOG_DATA", ORANGE,
          [](ButtonContext *ctx) -> bool
          { return ctx->currentEdit->getValue() == EditableValues::LOG_DATA; }));

  // Add and Clear
  posY += 10;
  posY += size.y + offset;
  posX = start.x;
  buttons.push_back(
      //buttons[9] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) += 500;
          },
          "+500", GREEN));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[10] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) += 100;
          },
          "+100", GREEN));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[11] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) += 60;
          },
          "+60", GREEN));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[12] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) += 10;
          },
          "+10", GREEN));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[13] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          {
            *(ctx->changeValue(ctx->currentEdit->getValue(), ctx->core->getTemperatureCurve())) = 0;
          },
          "0", GREEN));

  //Start / Stop
  posY += size.y + offset;
  posX = start.x + 3 * (size.x + offset);
  buttons.push_back(
      //buttons[14] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->core->start(); },
          "Start", ORANGE));
  posX += offset + size.x;
  buttons.push_back(
      //buttons[15] =
      Button(
          tft, context, {posX, posY}, size,
          [](Waveshare_ILI9486 *tft, ButtonContext *ctx)
          { ctx->core->stop(); },
          "Stop", ORANGE));
}