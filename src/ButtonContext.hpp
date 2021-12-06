#pragma once
#include "EditStatus.hpp"
#include "TemperatureCore.hpp"

typedef int *ValueGetter(EditableValues, TemperatureCurve *);

struct ButtonContext
{
    TemperatureCore *core;
    EditStatus *currentEdit;
    ValueGetter *changeValue;
};

typedef bool ActivityChecker(ButtonContext *);
typedef void OnClick(Waveshare_ILI9486 *, ButtonContext *);