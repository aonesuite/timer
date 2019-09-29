#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include "Display.h"

Display::Display(uint8_t clk, uint8_t dio)
{
  tm1637 = new TM1637(clk, dio);
  tm1637->set();
  tm1637->init();
  showTime(0, 0, false);
  clear();
}

void Display::clear()
{
  tm1637->point(false);
  tm1637->clearDisplay();
}

void Display::loop()
{
  tm1637->display(displayBuf);
  tm1637->point(showPoint);
}

void Display::showTime(int hour, int minute, boolean isOdd)
{
  showPoint = isOdd;
  displayBuf[0] = hour / 10;
  displayBuf[1] = hour % 10;
  displayBuf[2] = minute / 10;
  displayBuf[3] = minute % 10;
}

#endif
