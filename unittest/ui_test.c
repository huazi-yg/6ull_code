 #include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <display_manager.h>
#include <framebuffer.h>
#include <stdlib.h>

#include <font_manager.h>
#include <stdio.h>
#include "ui.h"

#define NULL ((void *)0)
int *ptr = NULL;


#define FONTDATAMAX 4096


int fd_fb;
struct fb_var_screeninfo var;	/* Current var */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;

int main(int argc, char **argv)
{
	pDispbuff ptBuffer;
	int error;
	Button tbutton;
	region tregion;
	
	if(argc != 2)
	{
		printf("Usage: %s <font_file> \n",argv[0]);
		return -1;
	}

	
	display_init();
	select_default_display("fb");
	init_default_display();
	
	ptBuffer = GetDisplayBuffer();

	//注册字体
	FontsRegister();	
	error = SelectAndInitFont("freetype",argv[1]);
	if(error)
	{
		
		printf("SelectAndInitFont error !\n");
		return -1;
	}
	
	//SetFontSize(font_size);

	tregion.iLeftupx = 200;
	tregion.ileftupy = 200;
	tregion.iheigh = 100;
	tregion.iwidth = 300;

	button_init("ui test",&tbutton,&tregion,NULL,NULL);

	tbutton.OnDraw(&tbutton,ptBuffer);

	while(1)
	{
		tbutton.onpressd(&tbutton,ptBuffer,NULL);
		sleep(2);
	}

	return 0;	
}


