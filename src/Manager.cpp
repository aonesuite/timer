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
  delay(1000);
  clock->loop();
  display->loop();
}

#endif