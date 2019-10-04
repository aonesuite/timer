#ifndef ENCODER_CPP
#define ENCODER_CPP

#include "Arduino.h"
#include "Encoder.h"
#include "config.h"

// 旋钮
// http://www.dfrobot.com.cn/goods-1421.html
const int encoderButtonPin = 7; // 旋钮开关
const int encoderPinA = 8;      //
const int encoderPinB = 9;      //

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
volatile long lastencoderValue = 0;

volatile boolean lastBtnStatus = 0;
volatile int updateCount = 0;
volatile int lastBtnChangeUpdateCount = 0;
volatile int lastLastBtnChangeUpdateCount = 0;
volatile boolean btnChanged = false;

volatile int btnHoldLoopCount = 0;

void setupEncoder()
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderButtonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on
}

// UpdateKnob 更新 360 度编码器开关数值
void updateEncoder()
{
  // -- 更新旋钮数据
  int MSB = digitalRead(encoderPinA); // MSB = most significant bit
  int LSB = digitalRead(encoderPinB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValue--;

  lastEncoded = encoded; // store this value for next time

  // -- 更新按钮数据
  boolean btnStatus = !digitalRead(encoderButtonPin);
  updateCount++;

  if (btnStatus == 1)
  {
    btnHoldLoopCount++;
  }
  else
  {
    btnHoldLoopCount = 0;
  }

  // 和上次相同，则不处理
  if (lastBtnStatus == btnStatus)
  {
    return;
  }

  if (btnStatus == 0)
  {
    // 按钮弹起了
    lastLastBtnChangeUpdateCount = lastBtnChangeUpdateCount;
    lastBtnChangeUpdateCount = updateCount;

    btnChanged = true;
  }

  lastBtnStatus = btnStatus;
}

// readEncoderValue 读取 360 度编码器数值
long readEncoderValue()
{
  return encoderValue / 4;
}

// 当前是否按下了按钮
boolean isBtnClicked()
{
  if (!btnChanged)
  {
    return false;
  }

  // 保证优先匹配双击
  if (lastBtnChangeUpdateCount - lastLastBtnChangeUpdateCount > (DOUBLE_CLICK_LOOP_COUNT + 10))
  {
    Serial.println("[ENCODER] CLICKED");
    btnChanged = false;
    return true;
  }

  return false;
}

// 是否触发了按钮双击动作
boolean isBtnDoubleClicked()
{
  if (!btnChanged)
  {
    return false;
  }

  boolean isNear = lastBtnChangeUpdateCount - lastLastBtnChangeUpdateCount < DOUBLE_CLICK_LOOP_COUNT;
  if (isNear)
  {
    Serial.println("[ENCODER] DOUBLE CLICKED");
    btnChanged = false;
    return true;
  }

  return false;
}

// 是否触发了按钮长按动作
boolean isBtnHoldLong()
{
  if (btnHoldLoopCount > HOLD_BTN_LOOP_COUNT)
  {
    btnHoldLoopCount = 0;
    return true;
  }

  return false;
}

#endif
