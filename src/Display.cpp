#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include "Display.h"
#include "config.h"

Display::Display()
{
  tm1637 = new TM1637(PIN_DISP_CLK, PIN_DISP_DIO);
  tm1637->set();
  tm1637->init();
  fillTime(0, 0, false);
  clear();
  flush();
}

void Display::flush()
{
  tm1637->point(showPoint);
  tm1637->display(displayBuf);

  // TODO 测试是否有必要？
  // if (showPoint != preShowPoint || displayBuf[0] != preDisplayBuf[0] || displayBuf[1] != preDisplayBuf[1] || displayBuf[2] != preDisplayBuf[2] || displayBuf[3] != preDisplayBuf[3])
  // {
  //   preShowPoint = showPoint;
  //   preDisplayBuf[0] = displayBuf[0];
  //   preDisplayBuf[1] = displayBuf[1];
  //   preDisplayBuf[2] = displayBuf[2];
  //   preDisplayBuf[3] = displayBuf[3];

  //   tm1637->point(showPoint);
  //   tm1637->display(displayBuf);
  // }
}

void Display::fillTime(unsigned char leftValue, unsigned char rightValue, boolean sp)
{
  showPoint = sp;
  displayBuf[0] = leftValue / 10;
  displayBuf[1] = leftValue % 10;
  displayBuf[2] = rightValue / 10;
  displayBuf[3] = rightValue % 10;
}

void Display::clear()
{
  tm1637->point(false);
  clearLeft();
  clearRight();
}

void Display::clearLeft()
{
  displayBuf[0] = 0x7f;
  displayBuf[1] = 0x7f;
}

void Display::clearRight()
{
  displayBuf[2] = 0x7f;
  displayBuf[3] = 0x7f;
}

#endif
