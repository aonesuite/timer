#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "Ticker.h"
#include "Clock.h"

Clock::Clock()
{
  mode = CLOCK_MODE_DIS;
  changed = false;
}

void Clock::update()
{
  changed = false;

  // 更新时间
  switch (mode)
  {
  case CLOCK_MODE_DIS:
    // 显示模式
    uint64_t s = getISRTimeCount() / 2;
    boolean newShowPoint = getISRTimeCount() % 2 == 0;

    if (newShowPoint != showPoint)
    {
      showPoint = newShowPoint;
      changed = true;
    }

    unsigned char newSecond = s % 60;
    if (newSecond != second)
    {
      second = newSecond;

      if (second == 0)
      {
        uint64_t tmp = s / 60;
        unsigned char newMinute = tmp % 60;

        if (newMinute != minute)
        {
          minute = newMinute;

          if (minute == 0)
          {
            hour = (tmp / 60) % 24;
          }
        }
      }
    }
    break;
  case CLOCK_MODE_SET:

    break;
  }
}

#endif