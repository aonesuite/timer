#ifndef OUTPUT_H
#define OUTPUT_H

#include "Arduino.h"

// 输出：蜂鸣器 + 震动器

// 初始化输出硬件
void initOutput();
// 输出循环
void loopOutout();

// 触发振动器震动，指定震动时间
void triggerVibration(uint32_t ms, int level);
// 触发蜂鸣器鸣叫，指定蜂鸣时间
void triggerBuzzer(uint32_t ms, int level);

#endif
