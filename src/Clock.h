#ifndef CLOCK_H
#define CLOCK_H

#include "Arduino.h"

#define CLOCK_MODE_DIS 0 // 显示模式
#define CLOCK_MODE_SET 1 // 设置模式

class Clock
{
public:
  Clock();
  void update();

  volatile int mode;
  volatile boolean showPoint;
  volatile unsigned char hour;
  volatile unsigned char minute;
  volatile unsigned char second;

  volatile boolean changed;
  volatile boolean isSetModeHour;

private:
  volatile unsigned char setModeBaseHour;
  volatile unsigned char setModeBaseMinute;
  volatile long setModeEncoderBaseHour;
  volatile long setModeEncoderBaseMinute;
};

#endif
