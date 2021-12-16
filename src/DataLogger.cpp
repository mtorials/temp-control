#include "DataLogger.hpp"
#include "Arduino.h"

DataLogger::DataLogger(int duration)
{
  // Serial.println("trying to null");
  reset(size);
  // for (int i = 0; i < duration; i++)
  // {
  //   this->values[i] = 0;
  //   Serial.print("Value is ");
  //   Serial.println(values[i]);
  //   Serial.print("null for ");
  //   Serial.println(i);
  // }
}

DataLogger::~DataLogger()
{
  delete this->values;
  // realloc(values, size * sizeof(short));
}

void DataLogger::setTempForT(int t, short temp)
{
  this->values[t] = temp;
}

void DataLogger::reset(int duration)
{
  // Serial.println("Resetting data logger.");
  // Serial.print("values pointer is null: ");
  // Serial.println(this->values == nullptr);
  this->size = duration;
  this->values = (short *)calloc(size, sizeof(short));
}

short DataLogger::getTempForT(int t)
{
  // Serial.print("t ");
  // Serial.print(t);
  // Serial.print(" : ");
  // Serial.print(values[t]);
  // Serial.println();
  return this->values[t];
}