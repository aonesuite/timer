#ifndef MANAGER_CPP
#define MANAGER_CPP

#include "Arduino.h"
#include "Manager.h"
#include "Ticker.h"

#define CLK 2
#define DIO 3

Manager::Manager()
{
  mode = MODE_CLOCK;
  clock = new Clock();
  display = new Display((uint8_t)CLK, (uint8_t)DIO);
  initISR();
}

void Manager::loop()
{
  delay(50);
  clock->loop();

  switch (mode)
  {
  case MODE_CLOCK:
    // 时钟模式
    if (clock->changed)
    {
      Serial.println(clock->second);
      display->showTime(clock->hour, clock->minute, clock->showPoint);
      display->refresh();
    }
    break;
  case MODE_COUNTDOWN:
    // 计时器模式
    break;
  }
}

#endif