#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H

#include "common.h"
#include "font_manager.h"
typedef struct 
{
 	int xres;
	int yres;
	int bpp;
	char *buf;
}Dispbuff,*pDispbuff;

typedef struct Display_Opr{
    char *name;
	int (*Device_Init)(void);
	int (*Device_Exit)(void);
    int (*GetBuffer)(pDispbuff ptDispbuff);
    int (*FlushRegion)(p_region pt_region,pDispbuff ptDispbuff);
    struct Display_Opr *ptNext;
}Display_Opr,*pDisplay_Opr;

void DrawFontBitMap(pFontBitMap ptFontBitMap,unsigned int color);
void RegisterDisplay(pDisplay_Opr ptDisplay_Opr);
void display_init(void);
int select_default_display(char *name);
int init_default_display(void);
int put_pixel(int x,int y,unsigned int color);
int flushregion(p_region pt_region,pDispbuff ptDispbuff);
pDispbuff GetDisplayBuffer(void);

void DrawRegion(p_region pt_region,unsigned int color);
void DrawTextRegionCenter(char *name,p_region pt_region,unsigned int color);
#endif