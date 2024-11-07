#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 	0
#define INPUT_TYPE_NET		1

typedef struct InputEvent {
	int iType;//输入类型
	struct timeval  ttimeval;
	//触摸屏
	int iX;
	int iY;
	unsigned int iPressure;
	//网络输入
	char str[1024];
}InputEvent,*pInputEvent;

typedef struct InputDevice {
	char *name;
	int (*GetInputEvent)(pInputEvent ptInputEvent);
	int (*Device_Init)(void);
	int (*Device_Exit)(void);
	struct InputDevice *ptNext;
}InputDevice,*pInputDevice;

#endif




