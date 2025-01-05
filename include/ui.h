#ifndef _UI_H
#define _UI_H

#include "common.h"
#include "display_manager.h"
#include "input_manager.h"

#define BUTTON_DEFAULT_COLOR 0xff0000
#define BUTTON_PRESSED_COLOR 0x00ff00
#define BUTTON_PERCENT_COLOR 0x0000ff

#define BUTTON_TEXT_COLOR    0x000000

struct Button;

typedef int (*ONDRAW_FUNC)(struct Button *ptButton,pDispbuff ptDispbuff);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton,pDispbuff ptDispbuff,pInputEvent ptInputEvent);

typedef struct Button {
	char *name;
	int status;
	region t_region;
	ONDRAW_FUNC OnDraw;
	ONPRESSED_FUNC onpressd;
}Button,*pButton;

int button_init(char *name,pButton ptButton,p_region pt_region,\
	ONDRAW_FUNC OnDraw,ONPRESSED_FUNC onpressd);


#endif
