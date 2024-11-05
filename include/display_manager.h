#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H

typedef struct 
{
 	int xres;
	int yres;
	int bpp;
	char *buf;
}Dispbuff,*pDispbuff;

typedef struct
{
    int iLeftupx;
    int ileftupy;
    int iwidth;
    int iheigh;
}region,*p_region;

typedef struct Display_Opr{
    char *name;
	int (*Device_Init)(void);
	int (*Device_Exit)(void);
    int (*GetBuffer)(pDispbuff ptDispbuff);
    int (*FlushRegion)(p_region pt_region,pDispbuff ptDispbuff);
    struct Display_Opr *ptNext;
}Display_Opr,*pDisplay_Opr;

void RegisterDisplay(pDisplay_Opr ptDisplay_Opr);
void display_init(void);
int select_default_display(char *name);
void init_default_display(void);
int put_pixel(int x,int y,unsigned int color);
static int flushregion(p_region pt_region,pDispbuff ptDispbuff);

	
#endif