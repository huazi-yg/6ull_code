#ifndef __LED_OPR_H
#define __LED_OPR_H

struct led_operations {
    int num;//led灯的个数
    int (*init)(int which);//初始化指定的LED
    int (*ctl)(int which, char status);//控制某个led亮或者是熄灭
};

struct led_operations *get_board_led_opr(void);
#endif
