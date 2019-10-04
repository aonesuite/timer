#ifndef COUNT_DOWN_H
#define COUNT_DOWN_H

#include "Arduino.h"

// 模式：显示 | 设置
#define COUNT_DOWN_MODE_DIS 0 // 显示模式
#define COUNT_DOWN_MODE_SET 1 // 设置模式

class CountDown
{
public:
  CountDown();
  void update();

  volatile unsigned char mode; // 模式 COUNT_DOWN_MODE_DIS | COUNT_DOWN_MODE_SET

  volatile boolean showPoint;    // 是否显示冒号
  volatile unsigned char minute; // 分钟
  volatile unsigned char second; // 秒钟

  volatile boolean pause;           // 是否暂停
  volatile boolean changed;         // 是否有变化
  volatile boolean isSetModeMinute; // true 设置模式下设置小时 false 设置模式下设置分钟

private:
  void initDisMode();
  void initSetMode();

  volatile uint64_t totalSeconds; // 倒计时目标秒数设置
  volatile uint64_t baseRunTime;  // 开始倒计时运行时的初始全局时间（s，非时间间隔）
  volatile uint64_t pauseTime;    // 暂停开始的初始全局时间（s，非时间间隔）

  volatile unsigned char setModeBaseMinute; // 设置分钟：分钟初始值
  volatile unsigned char setModeBaseSecond; // 设置秒钟：秒钟初始值
  volatile long setModeEncoderBaseMinute;   // 设置分钟：旋钮状态初始值
  volatile long setModeEncoderBaseSecond;   // 设置秒钟：旋钮状态初始值

  volatile boolean forceUpdateAll; // 强制刷新所有显示位：仅一次刷新有效，之后会重设为 false
};

#endif
