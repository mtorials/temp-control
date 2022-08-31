#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#include "Display.hpp"
#include "Heater.hpp"
#include "TemperatureCurve.hpp"
#include "TemperatureCore.hpp"
#include "Status.hpp"

#define I2C_ADDRESS (0x67)
#define MILLIS_IN_ONE_MIN 60000

Adafruit_MCP9600 mcp;

TemperatureCurve curve;
TemperatureCore *core;

Heater * heater;

Status status;
Display *display;

// predetermained touch calibration
TSConfigData configData = {
    .xMin = 58,
    .xMax = 975,
    .yMin = 39,
    .yMax = 979,
};

void setup()
{
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
  SPI.begin();

  if (!mcp.begin(I2C_ADDRESS))
  {
    Serial.println("Sensor not found. Check wiring!");
    Tft.begin();
    Tft.println("Sensnor not found. Check wiring!");
    while (1)
      ;
  }
  Serial.println("Found MCP9600!");
  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  mcp.setThermocoupleType(MCP9600_TYPE_S);
  mcp.setFilterCoefficient(3);
  mcp.enable(true);
  // display.update();
  // core.start();

  heater = new Heater();
  core = new TemperatureCore(&curve);
  display = new Display(core, &status);

  // Load touch calibration
  Tft.setTsConfigData(configData);

  display->begin();

  Serial.print("[main.cpp] Data logger is null: ");
  Serial.println(core->getDataLogger() == nullptr);
  Serial.println();
}

Status oldStatus = status;
int oldTime = core->getTime();

/* void manageTemperature()
{
  int delta = (status.currentTempereature - core->getTemperature());
  if (delta > 0)
  {
    status.heating = false;
  }
  if (delta < -TEMP_THRESHOLD)
  {
    status.heating = true;
  }
}
 */
void loop()
{
  core->setTime(millis() / MILLIS_IN_ONE_MIN);
  status.currentTempereature = mcp.readThermocouple();
  if (
      oldStatus.currentTempereature != status.currentTempereature ||
      oldStatus.heating != status.heating ||
      (core->getTime() != oldTime))
  {
    Serial.println(status.currentTempereature);
    display->update();
    heater->setHeating(status.currentTempereature, core->getTemperature());
  }
  display->loop();
  if (core->running())
  {
    //manageTemperature();
    // Log data
    heater->loop(&status);
    if (core->getTime() != oldTime)
    {
      core->getDataLogger()->setTempForT(core->getTime(), status.currentTempereature);
    }
  }
  else
  {
    status.heating = false;
  }
  if (status.heating)
  {
    digitalWrite(OUTPUT_PIN, HIGH);
  }
  else
  {
    digitalWrite(OUTPUT_PIN, LOW);
  }
  oldStatus = status;
  oldTime = core->getTime();

// Very bad for performance
#ifdef CALIBRATION
  if (!Serial.readString().startsWith("cal"))
    return;
  auto data = Tft.getTsConfigData();
  Serial.println("Display Config!");
  Serial.println(data.xMin);
  Serial.println(data.xMax);
  Serial.println(data.yMin);
  Serial.println(data.yMax);
  Serial.println("---------------------");
#endif
}
