#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "Ticker.h"
#include "Clock.h"
#include "Encoder.h"

Clock::Clock()
{
  mode = CLOCK_MODE_DIS;
  changed = false;
  isSetModeHour = true;
}

void Clock::update()
{
  changed = false;

  boolean forceUpdateAll = false;
  // 双击切换模式
  if (isBtnDoubleClicked())
  {
    mode = (mode == CLOCK_MODE_DIS) ? CLOCK_MODE_SET : CLOCK_MODE_DIS;

    if (mode == CLOCK_MODE_DIS)
    {
      // 进入显示模式，设置新时间
      setISRTimeCount(hour * 3600 + minute * 60);
      delay(1000);
    }
    else
    {
      // 进入设置模式，初始化数据
      showPoint = false;
      setModeBaseHour = hour;
      setModeBaseMinute = minute;
      setModeEncoderBaseHour = readEncoderValue();
      setModeEncoderBaseMinute = readEncoderValue();
    }

    changed = true;
    forceUpdateAll = true;

    Serial.print("SWITCH TO CLOCK MODE ");
    Serial.println(mode == CLOCK_MODE_DIS ? "DIS" : "SET");
  }

  // 更新时间
  if (mode == CLOCK_MODE_DIS)
  {
    // 显示模式
    uint64_t s = getISRTimeCount() / 2;
    boolean newShowPoint = getISRTimeCount() % 2 == 0;

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
          Serial.println("update minute");

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
    // 设置模式下，单击切换小时设置和分钟设置
    if (isBtnClicked())
    {
      isSetModeHour = !isSetModeHour;
      setModeBaseHour = hour;
      setModeBaseMinute = minute;
      setModeEncoderBaseHour = readEncoderValue();
      setModeEncoderBaseMinute = readEncoderValue();
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
      }
    }
  }
}

#endif