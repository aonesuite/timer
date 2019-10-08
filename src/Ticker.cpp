#ifndef TICKER_CPP
#define TICKER_CPP

#include <TimerOne.h>
#include "Ticker.h"

volatile uint64_t tickCount;

// 时钟中断间隔 250ms 不可修改！
#define TIME_SEG 250000
#define TIME_ONE_SECOND_COUNT 4

// 时钟中断函数
void loopISR()
{
  tickCount++;
}

// 初始化
void initISR()
{
  tickCount = 0;

  Timer1.initialize(TIME_SEG);
  Timer1.attachInterrupt(loopISR);
}

// 获取时钟终端计数
uint64_t getISRTimeCount()
{
  return tickCount;
}

// 获取当前时间（秒）
uint64_t getISRTimeSecond()
{
  return tickCount >> 2;
}

// 重设时间
void setISRTimeSecond(uint64_t seconds)
{
  tickCount = seconds * TIME_ONE_SECOND_COUNT;
}

// 判断当前是否是前半秒（主要用于冒号的闪烁）
boolean isFirstHalfSecond()
{
  return ((unsigned char)tickCount & 0x2) == 0;
}

#endif