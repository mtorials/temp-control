#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#include "Display.hpp"
#include "TemperatureCurve.hpp"
#include "TemperatureCore.hpp"
#include "Status.hpp"

#define I2C_ADDRESS (0x67)

Adafruit_MCP9600 mcp;

TemperatureCurve curve;
TemperatureCore core = TemperatureCore(&curve);

Status status;
Display display(&core, &status);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  display.begin();
  if (!mcp.begin(I2C_ADDRESS)) {
    Serial.println("Sensor not found. Check wiring!");
    while(1);
  }
  Serial.println("Found MCP9600!");
  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  mcp.setThermocoupleType(MCP9600_TYPE_S);
  mcp.setFilterCoefficient(3);
  mcp.enable(true);
}

Status oldStatus;

void loop() {
  oldStatus = status;
  status.currentTempereature = mcp.readThermocouple();
  if (oldStatus.currentTempereature != status.currentTempereature) {
    Serial.println(status.currentTempereature);
    display.update();
  }
  display.loop();
}