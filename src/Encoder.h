#ifndef ENCODER_H
#define ENCODER_H

// 旋钮按钮
// http://www.dfrobot.com.cn/goods-1421.html

// 初始化硬件
void initEncoder();
// 更新旋钮按钮信息
void updateEncoder();
// 读取旋钮数值
long readEncoderValue();
// 判断按钮是否按下
boolean isBtnClicked();
// 判断按钮是否双击
boolean isBtnDoubleClicked();
// 判断按钮是否长按
boolean isBtnHoldLong();

#endif
