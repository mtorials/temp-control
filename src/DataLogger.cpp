#include "DataLogger.hpp"
#include "Arduino.h"

DataLogger::DataLogger(int duration)
{
  // TODO
  this->size = duration;
  this->values = (short *)calloc(size, sizeof(short));
  Serial.println("trying to null");
  for (int i = 0; i < duration; i++)
  {
    this->values[i] = 1;
    Serial.print("Value is ");
    Serial.println(values[i]);
    Serial.print("null for ");
    Serial.println(i);
  }
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

void DataLogger::reset()
{
  for (int i = 0; i < this->size; i++)
  {
    this->values[i] = 0;
  }
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