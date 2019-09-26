#include <TimerOne.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00, 0x00, 0x00, 0x00};
unsigned char ClockPoint = 1; // 双跳灯
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;

bool alertEnable = false;

// 时钟引脚， Arduino Nano 只有 2，3 引脚支持 attachInterrupt
#define CLK 2
#define DIO 3
TM1637 tm1637(CLK, DIO);

const int buzzPinAnalog = A0;     // 蜂鸣器
const int vibratorPinAnalog = A1; // 震动模块

const int resetButtonPinAnalog = A2; // 重置开关按键

// 旋钮
const int encoderButtonPin = 7; // 旋钮开关
const int encoderPinA = 8;      //
const int encoderPinB = 9;      //

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("TIMER SETUP");

  pinMode(buzzPinAnalog, OUTPUT);     // 定义蜂鸣器引脚为输出引脚
  pinMode(vibratorPinAnalog, OUTPUT); // 定义震动模块引脚为输出引脚

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderButtonPin, INPUT);

  digitalWrite(encoderPinA, HIGH); // turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); // turn pullup resistor on

  // 时钟初始化，定义计时器中断服务程序
  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);         // timing for 500ms
  Timer1.attachInterrupt(TimingISR); // declare the interrupt serve routine:TimingISR
}

void loop()
{
  int sensorValue = analogRead(resetButtonPinAnalog);
  if (sensorValue == 0)
  {
    // TODO: 切换显示模式
  }

  // 在 loop 函数中代替 attachInterrupt 来监听更新 Encoder
  UpdateEncoder();

  // 输出 encoder value
  // TODO: 用于设置时间或报警时长
  Serial.println(ReadEncoderValue());

  // 重置按钮按下时，数值归零
  if (IsEncoderButtonPushDown())
  {
    Serial.println("you push button down!!!");

    ClockPoint = 1;
    halfsecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    Update = OFF;

    TimeUpdate();             // 更新时钟 TimeDisp
    tm1637.display(TimeDisp); // 更新数码管显示

    analogWrite(vibratorPinAnalog, 256); // 震动提示
    return;
  }
  else
  {
    Alert(false, false);
    analogWrite(vibratorPinAnalog, 0); // 震动提示
  }

  // 每分钟报警一次
  if (second == 59)
  {
    Alert(true, true);
    delay(50);
  }
  else
  {
    Alert(false, false);
  }

  if (Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
}

// TimingISR 计时器中断服务程序
void TimingISR()
{
  halfsecond++;
  Update = ON;
  if (halfsecond == 2)
  {
    second++;
    if (second == 60)
    {
      minute++;

      Serial.println("Another minute passed.");

      if (minute == 60)
      {
        hour++;
        if (hour == 24)
          hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
  ClockPoint = (~ClockPoint) & 0x01;
}

// TimeUpdate 更新数码管显示时间
void TimeUpdate(void)
{
  tm1637.point(ClockPoint ? POINT_ON : POINT_OFF);

  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  TimeDisp[2] = second / 10;
  TimeDisp[3] = second % 10;
  Update = OFF;
}

// UpdateEncoder 更新 360 度编码器开关数值
void UpdateEncoder()
{
  int MSB = digitalRead(encoderPinA); // MSB = most significant bit
  int LSB = digitalRead(encoderPinB); // LSB = least significant bit

  int encoded = (MSB << 1) | LSB;         // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValue--;

  lastEncoded = encoded; // store this value for next time
}

// ReadEncoderValue 读取 360 度编码器数值
long ReadEncoderValue(void)
{
  return encoderValue / 4;
}

// IsEncoderButtonPushDown 360 度编码器开关是否按下
boolean IsEncoderButtonPushDown(void)
{
  if (!digitalRead(encoderButtonPin))
  {
    delay(5);
    if (!digitalRead(encoderButtonPin))
      return true;
  }
  return false;
}

// Alert 蜂鸣和震动
void Alert(boolean buzz, boolean vibrator)
{
  if (alertEnable)
  {
    analogWrite(buzzPinAnalog, buzz ? 256 : 0);
    analogWrite(vibratorPinAnalog, vibrator ? 256 : 0);
  }
}
