#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "Ticker.h"
#include "Clock.h"
#include "Encoder.h"
#include "Output.h"

Clock::Clock()
{
  initDisMode();
}

void Clock::initDisMode()
{
  mode = CLOCK_MODE_DIS;
  forceUpdateAll = true;
  Serial.println("[CLOCK] enter display mode");
}

void Clock::initSetMode()
{
  mode = CLOCK_MODE_SET;
  forceUpdateAll = true;
  showPoint = false;                             // 设置模式冒号不闪烁
  setModeBaseHour = hour;                        // 记录进入设置模式时的初始小时
  setModeBaseMinute = minute;                    // 记录进入设置模式时的初始分钟
  setModeEncoderBaseHour = readEncoderValue();   // 记录进入小时设置时的初始旋钮数值
  setModeEncoderBaseMinute = readEncoderValue(); // 记录进入分钟设置时的初始旋钮数值
  Serial.println("[CLOCK] enter settings mode");
}

void Clock::update()
{
  changed = false;

  // 双击切换模式
  if (isBtnDoubleClicked())
  {
    if (mode == CLOCK_MODE_SET)
    {
      // 由设置模式进入显示模式，设置新时间
      setISRTimeSecond(hour * 3600 + minute * 60);
      delay(50);
      initDisMode();
    }
    else
    {
      isSetModeHour = true; // 默认处于设置小时状态
      initSetMode();
    }

    changed = true;
    triggerVibration(100, 255);
  }

  if (mode == CLOCK_MODE_DIS)
  {
    // 显示模式

    uint64_t s = getISRTimeSecond();
    boolean newShowPoint = isFirstHalfSecond();

    if (forceUpdateAll || newShowPoint != showPoint)
    {
      showPoint = newShowPoint;
      changed = true;
    }

    unsigned char newSecond = s % 60;
    if (forceUpdateAll || newSecond != second)
    {
      second = newSecond;

      if (forceUpdateAll || second == 0)
      {
        uint64_t tmp = s / 60;
        unsigned char newMinute = tmp % 60;

        if (forceUpdateAll || newMinute != minute)
        {
          minute = newMinute;

          if (forceUpdateAll || minute == 0)
          {
            hour = (tmp / 60) % 24;
          }
        }
      }
    }
  }
  else if (mode == CLOCK_MODE_SET)
  {
    // 设置模式

    // 单击切换小时设置和分钟设置
    if (isBtnClicked())
    {
      isSetModeHour = !isSetModeHour;
      initSetMode();
      triggerBuzzer(50, 255);
    }

    if (isSetModeHour)
    {
      // 设置小时
      long v = readEncoderValue() - setModeEncoderBaseHour;
      int newHour = (setModeBaseHour + v) % 24;
      if (newHour < 0)
      {
        newHour += 24;
      }
      if (newHour != hour)
      {
        hour = newHour;
        changed = true;
        triggerVibration(50, 255);
      }
    }
    else
    {
      // 设置分钟
      long v = readEncoderValue() - setModeEncoderBaseMinute;
      int newMin = (setModeBaseMinute + v) % 60;
      if (newMin < 0)
      {
        newMin += 60;
      }
      if (newMin != minute)
      {
        minute = newMin;
        changed = true;
        triggerVibration(50, 255);
      }
    }
  }

  forceUpdateAll = false;
}

#endif