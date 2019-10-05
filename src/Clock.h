#ifndef CLOCK_H
#define CLOCK_H

#include "Arduino.h"

// 模式：显示 | 设置
#define CLOCK_MODE_DIS 0 // 显示模式
#define CLOCK_MODE_SET 1 // 设置模式

class Clock
{
public:
  Clock();
  void update();
  void initDisMode();
  void initSetMode();

  volatile unsigned char mode; // 模式 CLOCK_MODE_DIS | CLOCK_MODE_SET

  volatile boolean showPoint;    // 是否显示冒号
  volatile unsigned char hour;   // 小时
  volatile unsigned char minute; // 分钟
  volatile unsigned char second; // 秒钟

  volatile boolean changed;       // 是否有变化
  volatile boolean isSetModeHour; // true 设置模式下设置小时 false 设置模式下设置分钟

private:
  volatile unsigned char setModeBaseHour;   // 设置小时：小时初始值
  volatile unsigned char setModeBaseMinute; // 设置分钟：分钟初始值
  volatile long setModeEncoderBaseHour;     // 设置小时：旋钮状态初始值
  volatile long setModeEncoderBaseMinute;   // 设置分钟：旋钮状态初始值

  volatile boolean forceUpdateAll; // 强制刷新所有显示位：仅一次刷新有效，之后会重设为 false
};

#endif
