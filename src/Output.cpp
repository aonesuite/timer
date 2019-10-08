#ifndef OUTPUT_CPP
#define OUTPUT_CPP

#include "Output.h"
#include "config.h"

// 注意：震动时间粒度要比较短，这里的时间通过 LOOP_DELAY 来估算

volatile uint32_t vibrRemainMS = 0;
volatile uint32_t buzzRemainMS = 0;

volatile int vibrLevel = 0; // 震动力度 0~255
volatile int buzzLevel = 0; // 蜂鸣响度 0~255

volatile int preVibrLevel = 0;
volatile int preBuzzLevel = 0;

// 初始化输出硬件
void initOutput()
{
  pinMode(PIN_ANALOG_VIBR, OUTPUT);
  pinMode(PIN_ANALOG_BUZZ, OUTPUT);
}

// 优化的输出，和之前相同则不输出
void optimizeAnalogWrite(uint8_t pin, int level, int preLevel)
{
  if (level ^ preLevel)
  {
    analogWrite(pin, level);
    Serial.println("[OUTPUT] write");
  }
}

// 输出循环
void loopOutout()
{
  if (vibrRemainMS > 0)
  {
    vibrRemainMS -= LOOP_DELAY;
  }
  else
  {
    vibrLevel = 0;
  }

  if (buzzRemainMS > 0)
  {
    buzzRemainMS -= LOOP_DELAY;
  }
  else
  {
    buzzLevel = 0;
  }

  optimizeAnalogWrite(PIN_ANALOG_VIBR, vibrLevel, preVibrLevel);
  preVibrLevel = vibrLevel;
  optimizeAnalogWrite(PIN_ANALOG_BUZZ, buzzLevel, preBuzzLevel);
  preBuzzLevel = buzzLevel;
}

// 触发振动器震动，指定震动时间
void triggerVibration(uint32_t ms, int level)
{
  vibrRemainMS = ms;
  vibrLevel = level;
}

// 触发蜂鸣器鸣叫，指定蜂鸣时间
void triggerBuzzer(uint32_t ms, int level)
{
  buzzRemainMS = ms;
  buzzLevel = level;
}

#endif
