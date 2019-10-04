#ifndef COUNT_DOWN_H
#define COUNT_DOWN_H

#include "Arduino.h"

#define COUNT_DOWN_MODE_DIS 0 // 显示模式
#define COUNT_DOWN_MODE_SET 1 // 设置模式

class CountDown
{
public:
  CountDown();
  void update();

  volatile int mode;
  volatile boolean showPoint;
  volatile unsigned char minute;
  volatile unsigned char second;

  volatile boolean changed;
  volatile boolean isSetModeMinute;
  volatile boolean pause;

private:
  void initModeDis();
  void initModeSet();
  int totalSeconds; // 倒计时设置
  uint64_t baseRunTime;
  uint64_t pauseTime;

  volatile unsigned char setModeBaseMinute;
  volatile unsigned char setModeBaseSecond;
  volatile long setModeEncoderBaseMinute;
  volatile long setModeEncoderBaseSecond;
};

#endif
