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

void Display::refresh()
{
  tm1637->display(displayBuf);
  tm1637->point(showPoint);
}

void Display::showTime(unsigned char hour, unsigned char minute, boolean sp)
{
  showPoint = sp;
  displayBuf[0] = hour / 10;
  displayBuf[1] = hour % 10;
  displayBuf[2] = minute / 10;
  displayBuf[3] = minute % 10;
}

void Display::hideLeft()
{
  displayBuf[0] = 0x7f;
  displayBuf[1] = 0x7f;
  // tm1637->display(0x00, 0x7f);
  // tm1637->display(0x01, 0x7f);
}

void Display::hideRight()
{
  displayBuf[2] = 0x7f;
  displayBuf[3] = 0x7f;
  // tm1637->display(0x02, 0x7f);
  // tm1637->display(0x03, 0x7f);
}

#endif
