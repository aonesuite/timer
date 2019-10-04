#ifndef COUNT_DOWN_CPP
#define COUNT_DOWN_CPP

#include "Ticker.h"
#include "CountDown.h"
#include "Encoder.h"
#include "config.h"
#include "Output.h"

CountDown::CountDown()
{
  mode = COUNT_DOWN_MODE_DIS;
  totalSeconds = DEFAULT_COUNT_DOWN_SECONDS;

  initDisMode();
}

// 初始化倒计时模式数据
void CountDown::initDisMode()
{
  forceUpdateAll = true;

  showPoint = true;
  minute = (totalSeconds / 60) % 60;
  second = totalSeconds % 60;

  baseRunTime = getISRTimeSecond();
  // 刚切换到显示模式暂停
  pause = true;
  pauseTime = baseRunTime;
  Serial.println("[COUNTDOWN] enter display mode");
}

// 初始化设置模式数据
void CountDown::initSetMode()
{
  forceUpdateAll = true;
  showPoint = false;
  setModeBaseMinute = minute;
  setModeBaseSecond = second;
  setModeEncoderBaseMinute = readEncoderValue();
  setModeEncoderBaseSecond = readEncoderValue();
  Serial.println("[COUNTDOWN] enter settings mode");
}

void CountDown::update()
{
  changed = false;

  // 双击切换模式
  if (isBtnDoubleClicked())
  {
    mode ^= 0x1; // 取反切换模式

    if (mode == COUNT_DOWN_MODE_DIS)
    {
      // 设置模式转为显示模式，根据设置设置目标倒计时秒数
      totalSeconds = minute * 60 + second;
      Serial.println(String("[COUNTDOWN] SET ") + " " + minute + ":" + second);
      initDisMode();
    }
    else
    {
      isSetModeMinute = true; // 默认处于设置分钟状态
      minute = (totalSeconds / 60) % 60;
      second = totalSeconds % 60;
      initSetMode();
    }

    changed = true;
    triggerVibration(100, 255);
  }

  // 更新时间
  if (mode == COUNT_DOWN_MODE_DIS)
  {
    // 显示模式

    // 显示模式下，单击暂停
    if (isBtnClicked())
    {
      pause = !pause;
      if (pause)
      {
        // 暂停时记录暂停的时间
        pauseTime = getISRTimeSecond();
        Serial.println("[COUNTDOWN] PAUSE");
      }
      else
      {
        // 恢复时，更新 base 时间
        baseRunTime += getISRTimeSecond() - pauseTime;
        Serial.println("[COUNTDOWN] RESUME");
      }
      triggerBuzzer(50, 255);
    }

    // 暂停状态直接返回
    if (pause && !forceUpdateAll)
    {
      return;
    }

    // 运行
    uint64_t s = getISRTimeSecond();
    boolean newShowPoint = isFirstHalfSecond();

    if (forceUpdateAll || newShowPoint != showPoint)
    {
      showPoint = newShowPoint;
      changed = true;
    }

    int remainSeconds = totalSeconds - (s - baseRunTime);
    if (remainSeconds < 0)
    {
      // 倒计时结束，回到初始状态
      triggerBuzzer(500, 255);
      triggerVibration(500, 255);
      initDisMode();
      return;
    }

    unsigned char newSecond = remainSeconds % 60;
    if (forceUpdateAll || newSecond != second)
    {
      second = newSecond;
      if (forceUpdateAll || second == 59)
      {
        uint64_t tmp = remainSeconds / 60;
        unsigned char newMinute = tmp % 60;

        if (forceUpdateAll || newMinute != minute)
        {
          minute = newMinute;
        }
      }
    }
  }
  else if (mode == COUNT_DOWN_MODE_SET)
  {
    // 设置模式下，单击切换小时设置和分钟设置
    if (isBtnClicked())
    {
      isSetModeMinute = !isSetModeMinute;
      initSetMode();
      triggerBuzzer(50, 255);
    }

    if (isSetModeMinute)
    {
      // 设置分钟
      long v = readEncoderValue() - setModeEncoderBaseMinute;
      int newMinute = (setModeBaseMinute + v) % 60;
      if (newMinute < 0)
      {
        newMinute += 60;
      }
      if (newMinute != minute)
      {
        minute = newMinute;
        changed = true;
        triggerVibration(50, 255);
      }
    }
    else
    {
      // 设置秒钟
      long v = readEncoderValue() - setModeEncoderBaseSecond;
      int newSec = (setModeBaseSecond + v) % 60;
      if (newSec < 0)
      {
        newSec += 60;
      }
      if (newSec != second)
      {
        second = newSec;
        changed = true;
        triggerVibration(50, 255);
      }
    }
  }

  forceUpdateAll = false;
}

#endif