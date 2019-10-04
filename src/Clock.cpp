#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "Ticker.h"
#include "Clock.h"
#include "Encoder.h"

Clock::Clock()
{
  mode = CLOCK_MODE_DIS;
  changed = false;
  setModeBase = 0;
}

void Clock::update()
{
  changed = false;

  boolean forceUpdateAll = false;
  // 双击切换模式
  if (isBtnDoubleClicked())
  {
    mode = (mode == CLOCK_MODE_DIS) ? CLOCK_MODE_SET : CLOCK_MODE_DIS;

    changed = true;
    forceUpdateAll = true;

    if (mode == CLOCK_MODE_DIS)
    {
      // 设置新时间
      // TODO
      Serial.println((long)getISRTimeCount());
    }
    else
    {
      showPoint = false;
      setModeBase = 0;
    }
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
    long v = readEncoderValue();
    if (setModeBase == 0)
    {
      setModeBase = v;
    }

    v -= setModeBase;
    if (v == setModeLastV)
    {
      return;
    }

    hour = (hour + v) % 24;

    changed = true;
    setModeLastV = v;
  }
}

#endif