#ifndef MANAGER_H
#define MANAGER_H

#include "Clock.h"
#include "CountDown.h"
#include "Display.h"
#include "config.h"

// 模式：时钟 | 倒计时
#define MODE_CLOCK 0
#define MODE_COUNTDOWN 1

class Manager
{
public:
  Manager();
  void loop();

private:
  volatile unsigned char mode; // 模式 MODE_CLOCK | MODE_COUNTDOWN
  volatile int holdNoBlinkTimes;

  Display *display;     // 显示屏
  Clock *clock;         // 时钟控制服务
  CountDown *countDown; // 倒计时控制服务
};

#endif
