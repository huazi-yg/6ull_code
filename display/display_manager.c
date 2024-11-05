#include <stdio.h>
#include <string.h>

#include "display_manager.h"
#include "framebuffer.h"

//管理底层的LCD，WEB等
static pDisplay_Opr g_display_dev = NULL;
static pDisplay_Opr g_display_default = NULL;
static Dispbuff g_display_buff;

static int line_width;
static int pixel_width;


int put_pixel(int x,int y,unsigned int color)
{
	unsigned char *pen_8 = (unsigned char *)(g_display_buff.buf+y*line_width+x*pixel_width);
	unsigned short *pen_16; 
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_display_buff.bpp)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_display_buff.bpp);
			return -1;
			break;
		}
	}
	return 0;

}

//注册显示函数
void RegisterDisplay(pDisplay_Opr ptDisplay_Opr)
{
	ptDisplay_Opr->ptNext = g_display_dev;
	g_display_dev = ptDisplay_Opr;
}

//选择显示模块
int select_default_display(char *name)
{
	pDisplay_Opr p_temp = g_display_dev;
	while(p_temp)
	{
		if(0 == strcmp(name,p_temp->name))
		{
			g_display_default = p_temp;
			return 0;
		}
		p_temp = p_temp->ptNext;
	}
	return -1;
}

int init_default_display(void)
{
	int ret;
	ret = g_display_default->Device_Init();
	if(ret < 0)
	{
		printf("Device_Init error\n");
		return -1;
	}
	ret = g_display_default->GetBuffer(&g_display_buff);
	if(ret)
	{
		printf("GetBuffer error\n");
		return -1;
	}

	line_width 	= g_display_buff.xres * g_display_buff.bpp/8 ;
	pixel_width = g_display_buff.bpp/8 ;
	return 0;
}

pDispbuff GetDisplayBuffer(void)
{
	return &g_display_buff;
}

int flushregion(p_region pt_region,pDispbuff ptDispbuff)
{
	g_display_dev->FlushRegion(pt_region,ptDispbuff);
	return 0;
}


void display_init(void)
{
	FramebufferInit();
}
