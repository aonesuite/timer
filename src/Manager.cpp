#ifndef MANAGER_CPP
#define MANAGER_CPP

#include "Arduino.h"
#include "Manager.h"
#include "Ticker.h"
#include "Encoder.h"
#include "Output.h"

// 初始化系统
Manager::Manager()
{
  mode = MODE_CLOCK;
  holdNoBlinkTimes = 0;

  display = new Display();
  clock = new Clock();
  countDown = new CountDown();

  initISR();     // 初始化时钟中断
  initEncoder(); // 初始化旋钮
  initOutput();  // 初始化输出
}

// 主逻辑循环
void Manager::loop()
{
  delay(LOOP_DELAY);
  updateEncoder();
  if (holdNoBlinkTimes > 0)
  {
    holdNoBlinkTimes--;
  }

  // 旋钮长按切换模式
  if (isBtnHoldLong())
  {
    mode ^= 0x1; // 取反切换模式

    clock->initDisMode();
    countDown->initDisMode();

    Serial.println(String("[MANAGER] SWITCH MODE TO: ") + mode == MODE_CLOCK ? "CLOCK" : "COUNTDOWN");
    triggerVibration(1000, 255);
    triggerBuzzer(500, 255);
  }

  switch (mode)
  {
  case MODE_CLOCK:
    // 时钟模式
    clock->update();

    if (clock->changed)
    {
      // 如果数据有变动，刷新显示屏
      display->fillTime(clock->hour, clock->minute, clock->showPoint);
      // display->fillTime(clock->minute, clock->second, clock->showPoint);
      display->flush();
      holdNoBlinkTimes = HOLD_NO_BLINK_LOOP_COUNT;
    }
    else if (clock->mode == CLOCK_MODE_SET && holdNoBlinkTimes <= 0)
    {
      // 数据无变动，但在设置模式下，配置闪烁
      display->fillTime(clock->hour, clock->minute, clock->showPoint);
      // 设置模式下，闪烁设置项（小时|分钟）
      if (isFirstHalfSecond())
      {
        clock->isSetModeHour ? display->clearLeft() : display->clearRight();
      }
      display->flush();
    }

    break;
  case MODE_COUNTDOWN:
    // 计时器模式
    countDown->update();

    if (countDown->changed)
    {
      display->fillTime(countDown->minute, countDown->second, countDown->showPoint);
      display->flush();
      holdNoBlinkTimes = HOLD_NO_BLINK_LOOP_COUNT;
    }
    else if (countDown->mode == COUNT_DOWN_MODE_SET && holdNoBlinkTimes <= 0)
    {
      display->fillTime(countDown->minute, countDown->second, countDown->showPoint);
      // 设置模式下，闪烁设置项（秒钟|分钟）
      if (isFirstHalfSecond())
      {
        countDown->isSetModeMinute ? display->clearLeft() : display->clearRight();
      }
      display->flush();
    }
    break;
  }

  loopOutout();
}

#endif