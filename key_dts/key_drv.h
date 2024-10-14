#ifndef __LED_DRV_H
#define __LED_DRV_H

#include "led_opr.h"

//创造led类
void led_class_create_device(int minor);
//销毁led类
void led_class_destroy_device(int minor);
//注册led操作结构体
void register_led_operations(struct led_operations * opr);

#endif
