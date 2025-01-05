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
#include <page_manager.h>
#include <input_manager.h>


int main(int argc, char **argv)
{
	int error;

	if(argc != 2)
	{
		printf("Usage: %s <font_file> \n",argv[0]);
		return -1;
	}

	display_init();
	select_default_display("fb");
	init_default_display();

	//注册字体
	FontsRegister();	
	error = SelectAndInitFont("freetype",argv[1]);
	if(error)
	{
		
		printf("SelectAndInitFont error !\n");
		return -1;
	}
	
	//SetFontSize(font_size);

	input_init();//初始化输入
	input_device_init();//初始化设备

	//初始化页面系统
	PagesRegister();
	/* 运行业务系统的主页面 */
	Page("main")->Run(NULL);

	return 0;	
}


