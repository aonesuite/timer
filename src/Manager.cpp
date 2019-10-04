#ifndef MANAGER_CPP
#define MANAGER_CPP

#include "Arduino.h"
#include "Manager.h"
#include "Ticker.h"
#include "Encoder.h"

#define CLK 2
#define DIO 3

Manager::Manager()
{
  mode = MODE_CLOCK;
  clock = new Clock();
  display = new Display((uint8_t)CLK, (uint8_t)DIO);
  initISR();
  setupEncoder();
}

void Manager::loop()
{
  delay(LOOP_DELAY);
  updateEncoder();

  if (isBtnHoldLong())
  {
    mode = mode == MODE_CLOCK ? MODE_COUNTDOWN : MODE_CLOCK;
    Serial.print("[MANAGER] SWITCH MODE TO: ");
    Serial.println(mode == MODE_CLOCK ? "CLOCK" : "COUNTDOWN");
  }

  switch (mode)
  {
  case MODE_CLOCK:
    // 时钟模式
    clock->update();

    if (clock->changed)
    {
      display->showTime(clock->hour, clock->minute, clock->showPoint);
      display->refresh();
    }
    else if (clock->mode == CLOCK_MODE_SET)
    {
      display->showTime(clock->hour, clock->minute, clock->showPoint);
      // 设置模式下，闪烁设置项（小时|分钟）
      boolean blinkShow = getISRTimeCount() % 2 == 0;
      if (!blinkShow)
      {
        // 隐藏分钟或小时
        clock->isSetModeHour ? display->hideLeft() : display->hideRight();
      }

      display->refresh();
    }

    break;
  case MODE_COUNTDOWN:
    // 计时器模式
    display->clear();
    display->refresh();
    break;
  }
}

#endif