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
  int64_t dataBuf = (showPoint << 32) | (displayBuf[0] << 24) | (displayBuf[1] << 16) | (displayBuf[2] << 8) | displayBuf[3];
  if (preDataBuf ^ dataBuf)
  {
    // 有不同，再输出
    tm1637->point(showPoint);
    tm1637->display(displayBuf);
  }
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
