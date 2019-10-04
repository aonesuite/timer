#ifndef CONFIG_H
#define CONFIG_H

// 显示屏口定义
#define PIN_DISP_CLK 2
#define PIN_DISP_DIO 3

// 旋钮按钮接口定义
#define PIN_ENCODER_BTN 7 // 旋钮按钮
#define PIN_ENCODER_A 8
#define PIN_ENCODER_B 9

#define LOOP_DELAY 10                 // 主循环间隔，默认 loop = 10ms
#define DOUBLE_CLICK_TIME_GAP 500     // 按钮双击最大间隔时间，默认 1000ms
#define HOLD_BTN_TIME 2000            // 长按动作最小时长定义，默认 2000ms
#define DEFAULT_COUNT_DOWN_SECONDS 90 // 默认倒计时，秒

#define PIN_ANALOG_BUZZ A0 // 蜂鸣器
#define PIN_ANALOG_VIBR A1 // 震动器

#endif
