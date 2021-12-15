#pragma once

class DataLogger
{
private:
  short *values;
  short size;

public:
  DataLogger(int duration);
  ~DataLogger();
  short getTempForT(int t);
  void setTempForT(int t, short temp);

  short *getValues()
  {
    return this->values;
  }

  void reset();
};