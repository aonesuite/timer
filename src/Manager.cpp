#ifndef MANAGER_CPP
#define MANAGER_CPP

#include "Arduino.h"
#include "Manager.h"
#include "Ticker.h"
#include "Encoder.h"

#define CLK 2
#define DIO 3

Manager::Manager()
{
  mode = MODE_CLOCK;
  clock = new Clock();
  display = new Display((uint8_t)CLK, (uint8_t)DIO);
  initISR();
  setupEncoder();
}

void Manager::loop()
{
  delay(50);
  updateEncoder();

  switch (mode)
  {
  case MODE_CLOCK:
    // 时钟模式
    clock->update();

    if (clock->changed)
    {
      display->showTime(clock->hour, clock->minute, clock->showPoint);
      // display->showTime(clock->minute, clock->second, clock->showPoint);
      display->refresh();
    }
    break;
  case MODE_COUNTDOWN:
    // 计时器模式
    break;
  }
}

#endif