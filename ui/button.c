
#include "ui.h"
#include <stdio.h>

static int DefaultOnDraw(struct Button *ptButton,pDispbuff ptDispbuff)
{
	//绘制底色
	DrawRegion(&ptButton->t_region,BUTTON_DEFAULT_COLOR);
	//增加文字
	/* 居中写文字 */
	SetFontSize(14);
	DrawTextRegionCenter(ptButton->name,&ptButton->t_region,BUTTON_TEXT_COLOR);
	flushregion(&ptButton->t_region,ptDispbuff);

	return 0;
}

static int default_onpressd(struct Button *ptButton,pDispbuff ptDispbuff,pInputEvent ptInputEvent)
{
	unsigned int dw_color = BUTTON_DEFAULT_COLOR;
	ptButton->status = !ptButton->status;
	if(ptButton->status)
	{
		dw_color = BUTTON_PRESSED_COLOR;
	}
	printf("status %d \n",ptButton->status);
		//绘制底色
	DrawRegion(&ptButton->t_region,dw_color);
	//增加文字
	DrawTextRegionCenter(ptButton->name,&ptButton->t_region,BUTTON_TEXT_COLOR);
	flushregion(&ptButton->t_region,ptDispbuff);

	return 0;
}

int button_init(char *name,pButton ptButton,p_region pt_region,\
	ONDRAW_FUNC OnDraw,ONPRESSED_FUNC onpressd)
{
	ptButton->status = 0;
	ptButton->name = name;
	if(pt_region)
	{
		ptButton->t_region = *pt_region;
	}
	ptButton->OnDraw = OnDraw ? OnDraw : DefaultOnDraw;
	ptButton->onpressd = onpressd ? onpressd : default_onpressd;
	return 0;
}

