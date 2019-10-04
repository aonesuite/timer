#ifndef ENCODER_CPP
#define ENCODER_CPP

#include "Arduino.h"
#include "Encoder.h"
#include "config.h"
#include "Ticker.h"

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
volatile long lastencoderValue = 0;

volatile boolean preBtnStatus = 0;
volatile uint64_t btnHoldStartTime = 0; // 记录按钮持续按下的时间
volatile boolean isHoldStatusRead = false;

volatile boolean btnChanged = false;
volatile uint64_t preBtnChangedTime = 0;    // 上一次按钮触发的时间
volatile uint64_t prePreBtnChangedTime = 0; // 前一次按钮触发的时间

boolean btnHoldJustRead = false; // 标记长按刚刚结束，下一次抬起按钮前

// 初始化硬件
void initEncoder()
{
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
  pinMode(PIN_ENCODER_BTN, INPUT);

  digitalWrite(PIN_ENCODER_A, HIGH); //turn pullup resistor on
  digitalWrite(PIN_ENCODER_B, HIGH); //turn pullup resistor on
}

// 更新旋钮数值、按钮状态
void updateEncoder()
{
  // 更新旋钮数据
  {
    int MSB = digitalRead(PIN_ENCODER_A); // MSB = most significant bit
    int LSB = digitalRead(PIN_ENCODER_B); // LSB = least significant bit

    int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
    int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
      encoderValue++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
      encoderValue--;

    lastEncoded = encoded; // store this value for next time
  }

  // 更新按钮状态
  {
    boolean btnStatus = !digitalRead(PIN_ENCODER_BTN); // 1 为按下 0 为抬起

    if (btnStatus == 1)
    {
      // 按钮按下
      if (btnHoldStartTime == 0)
      {
        // 按下连续状态记录
        btnHoldStartTime = getISRTimeCount();
      }
    }
    else
    {
      // 按钮抬起
      btnHoldStartTime = 0;     // 长按起始时间清空
      isHoldStatusRead = false; // 长按读取状态清空
    }

    if (preBtnStatus == btnStatus)
    {
      // 按钮状态和上次相同，则不认定触发过单击
      return;
    }

    if (btnStatus == 0 && !btnHoldJustRead)
    {
      // 按钮弹起了，认定触发了单击
      // 长按刚刚抬起这一次不算
      btnChanged = true;
      prePreBtnChangedTime = preBtnChangedTime;
      preBtnChangedTime = getISRTimeCount();
    }

    btnHoldJustRead = false;

    preBtnStatus = btnStatus;
  }
}

// 读取 360 度编码器数值
long readEncoderValue()
{
  return encoderValue / 4;
}

// 判断按钮在是否有被按过一次
boolean isBtnClicked()
{
  if (!btnChanged)
  {
    return false;
  }

  // 小于等于 2 个时间间隔（500ms）不认定为单击（目的是优先匹配双击）
  if (getISRTimeCount() - preBtnChangedTime <= 2)
  {
    return false;
  }

  // 按钮按下状态只能被读取一次
  btnChanged = false;

  Serial.println("[ENCODER] CLICKED");
  return true;
}

// 是否触发了按钮双击动作
boolean isBtnDoubleClicked()
{
  if (!btnChanged)
  {
    return false;
  }

  // 两次触发间隔大于 2 个时间间隔（500ms）无效
  if (preBtnChangedTime - prePreBtnChangedTime > 2)
  {
    return false;
  }

  // 按钮双击状态只能读取一次
  btnChanged = false;

  Serial.println("[ENCODER] DOUBLE CLICKED");
  return true;
}

// 是否触发了按钮长按动作
boolean isBtnHoldLong()
{
  if (btnHoldStartTime == 0)
  {
    return false;
  }

  // 如果长按状态被读取过，则不再重复读取
  if (isHoldStatusRead)
  {
    return false;
  }

  // 持续按下时间超过 8 个时间单位（2s），判定为长按动作
  if (getISRTimeCount() - btnHoldStartTime > 8)
  {
    isHoldStatusRead = true;
    btnChanged = false;
    btnHoldJustRead = true;
    return true;
  }

  return false;
}

#endif
