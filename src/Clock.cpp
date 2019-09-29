#ifndef CLOCK_CPP
#define CLOCK_CPP

#include "Clock.h"
#include "Ticker.h"

Clock::Clock()
{
  Serial.println("Clock constructor");
}

void Clock::loop()
{
  Serial.println("Clock loop");
  Serial.println((int)getISRTimeCount());
}

#endif