#ifndef TICKER_CPP
#define TICKER_CPP

#include <TimerOne.h>
#include "Ticker.h"

volatile uint64_t tickCount;

void initISR()
{
  tickCount = 0;

  Timer1.initialize(500000);       // timing for 500ms
  Timer1.attachInterrupt(loopISR); // declare the interrupt serve routine:TimingISR
}

// 时钟中断
void loopISR()
{
  tickCount++;
}

uint64_t getISRTimeCount()
{
  return tickCount;
}

void setISRTimeCount(uint64_t seconds)
{
  tickCount = seconds * 2;
}

#endif