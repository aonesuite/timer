#ifndef DISPLAY_H
#define DISPLAY_H

#include "TM1637.h"

class Display
{
public:
  Display();
  void flush();                                                                 // 调用该函数才会将缓存中的数据显示到显示器
  void fillTime(unsigned char leftValue, unsigned char rightValue, boolean sp); // 写时间信息到缓存
  void clear();                                                                 // 清空所有数据
  void clearLeft();                                                             // 清空左侧数据
  void clearRight();                                                            // 清空右侧数据

private:
  TM1637 *tm1637;
  int8_t displayBuf[4];
  boolean showPoint;
  int64_t preDataBuf;
};

#endif
