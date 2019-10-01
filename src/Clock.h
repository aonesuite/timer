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

  int mode;
  boolean showPoint;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;

  boolean changed;
};

#endif
