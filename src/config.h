#ifndef CONFIG_H
#define CONFIG_H

#define LOOP_DELAY 10 // 主循环间隔，默认 loop = 10ms

#define DOUBLE_CLICK_TIME_GAP 500  // 按钮双击最大间隔时间，默认 500ms
#define DOUBLE_CLICK_LOOP_COUNT 50 // 按钮双击最大间隔 loop 数量 (DOUBLE_CLICK_TIME_GAP/LOOP_DELAY)

#define HOLD_BTN_TIME 2000      // 长按动作最小时长定义，默认 2000ms
#define HOLD_BTN_LOOP_COUNT 200 // 长按动作最小 loop 数量 (HOLD_BTN_TIME/LOOP_DELAY)

#define DEFAULT_COUNT_DOWN_SECONDS 90 // 默认倒计时，秒

#endif
