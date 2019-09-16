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

#define CLK 3 //pins definitions for TM1637 and can be changed to other ports
#define DIO 2
TM1637 tm1637(CLK, DIO);

const int resetSwitchPin = 4; // 重置开关按键
const int buzzPin = 5;        // 蜂鸣器
const int VibPin = 6;         // 震动模块

void setup()
{
  Serial.begin(9600);
  Serial.println("TIMER SETUP");

  pinMode(resetSwitchPin, INPUT); // 定义重置开关按键引脚为输入引脚
  pinMode(buzzPin, OUTPUT);       // 定义蜂鸣器引脚为输出引脚
  pinMode(VibPin, OUTPUT);        // 定义震动模块引脚为输出引脚

  tm1637.set();
  tm1637.init();
  Timer1.initialize(500000);         //timing for 500ms
  Timer1.attachInterrupt(TimingISR); //declare the interrupt serve routine:TimingISR
}

void loop()
{
  int val = digitalRead(resetSwitchPin); //读取输入值
  if (val == HIGH)                       // 检查输入是否为高，这里高为按下
  {
    Serial.println("resetSwitchPin HIGH");
    digitalWrite(buzzPin, HIGH); // 开启蜂鸣器
    digitalWrite(VibPin, HIGH);  // 开启震动器

    ClockPoint = 1;
    halfsecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    Update = OFF;
  }
  else
  {
    digitalWrite(buzzPin, LOW); // 关闭蜂鸣器
    digitalWrite(VibPin, LOW);  // 关闭震动器
  }

  Serial.print("second: ");
  Serial.println(second);

  if (second == 59)
  {
    digitalWrite(buzzPin, HIGH);
    digitalWrite(VibPin, HIGH);
    delay(50);
  }
  else
  {
    digitalWrite(buzzPin, LOW);
    digitalWrite(VibPin, LOW);
  }

  if (Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
}

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

void TimeUpdate(void)
{
  tm1637.point(ClockPoint ? POINT_ON : POINT_OFF);

  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  TimeDisp[2] = second / 10;
  TimeDisp[3] = second % 10;
  Update = OFF;

  Serial.print("Time Display: ");
  Serial.print(TimeDisp[0]);
  Serial.print(TimeDisp[1]);
  Serial.print(":");
  Serial.print(TimeDisp[2]);
  Serial.println(TimeDisp[3]);
}
