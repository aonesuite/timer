#ifndef TICKER_H
#define TICKER_H

#include "Arduino.h"

// 初始化
void initISR();
// 获取时钟终端计数
uint64_t getISRTimeCount();
// 获取当前时间（秒）
uint64_t getISRTimeSecond();
// 重设时间
void setISRTimeSecond(uint64_t seconds);
// 判断当前是否是前半秒（用户闪烁）
boolean isFirstHalfSecond();

#endif
