#ifndef DISPLAY_H
#define DISPLAY_H

#include "TM1637.h"

class Display
{
public:
  Display(uint8_t clk, uint8_t dio);
  void clear();
  void showTime(unsigned char hour, unsigned char minute, boolean isOdd); // 显示时间
  void refresh();

private:
  TM1637 *tm1637;
  int8_t displayBuf[4];
  boolean showPoint;
};

#endif
