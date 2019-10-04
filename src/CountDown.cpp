#ifndef COUNT_DOWN_CPP
#define COUNT_DOWN_CPP

#include "Ticker.h"
#include "CountDown.h"
#include "Encoder.h"
#include "config.h"

CountDown::CountDown()
{
  mode = COUNT_DOWN_MODE_SET;
  changed = false;
  totalSeconds = DEFAULT_COUNT_DOWN_SECONDS;
  initModeSet();
}

// 初始化设置模式数据
void CountDown::initModeSet()
{

  second = totalSeconds % 60;
  minute = totalSeconds / 60 % 60;

  showPoint = false;
  isSetModeMinute = true;
  setModeBaseMinute = minute;
  setModeBaseSecond = second;
  setModeEncoderBaseMinute = readEncoderValue();
  setModeEncoderBaseSecond = readEncoderValue();
}

// 初始化倒计时模式数据
void CountDown::initModeDis()
{
  totalSeconds = minute * 60 + second;
  Serial.println("设置完成");
  Serial.println(totalSeconds);
  delay(500);
  baseRunTime = getISRTimeCount();
  pause = true;
}

void CountDown::update()
{
  changed = false;

  boolean forceUpdateAll = false;
  // 双击切换模式
  if (isBtnDoubleClicked())
  {
    mode = (mode == COUNT_DOWN_MODE_DIS) ? COUNT_DOWN_MODE_SET : COUNT_DOWN_MODE_DIS;

    if (mode == COUNT_DOWN_MODE_DIS)
    {
      initModeDis();
    }
    else
    {
      initModeSet();
    }

    changed = true;
    forceUpdateAll = true;

    Serial.print("[COUNT_DOWN] SWITCH MODE TO: ");
    Serial.println(mode == COUNT_DOWN_MODE_DIS ? "DIS" : "SET");
  }

  // 更新时间
  if (mode == COUNT_DOWN_MODE_DIS)
  {
    // 倒计时模式下，单击暂停
    if (isBtnClicked())
    {
      pause = !pause;
      if (pause)
      {
        // 暂停时记录暂停的时间
        pauseTime = getISRTimeCount();
      }
      {
        // 恢复时，更新 base 时间
        baseRunTime += getISRTimeCount() - pauseTime;
        delay(500);
      }
    }

    if (pause)
    {
      return;
    }

    // 显示模式
    uint64_t t = getISRTimeCount();
    boolean newShowPoint = getISRTimeCount() % 2 == 0;

    if (forceUpdateAll || newShowPoint != showPoint)
    {
      showPoint = newShowPoint;
      changed = true;
    }

    int remainSeconds = totalSeconds - ((t - baseRunTime) >> 1);

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
      setModeBaseSecond = second;
      setModeBaseMinute = minute;
      setModeEncoderBaseSecond = readEncoderValue();
      setModeEncoderBaseMinute = readEncoderValue();
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
      }
    }
  }
}

#endif