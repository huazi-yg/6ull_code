#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "display_manager.h"

static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

static int fb_device_init(void);
static int fb_device_exit(void);
static int fb_get_buffer(pDispbuff ptDispbuff);
static int fb_flushregion(p_region pt_region,pDispbuff ptDispbuff);


static Display_Opr g_tFramebufferOpr = {
	.name = "fb",
	.Device_Init = fb_device_init,
	.Device_Exit = fb_device_exit,
	.GetBuffer = fb_get_buffer,
	.FlushRegion = fb_flushregion,
};


static int fb_device_init(void)
{
	
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}

	return 0;
}

static int fb_device_exit(void)
{
	munmap(fb_base , screen_size);
	close(fd_fb);
	return 0;
}

//可以返回LCD的framebuffer，也可以通过malloc分配一块内存，fb_flushregion刷新到
static int fb_get_buffer(pDispbuff ptDispbuff)
{
	ptDispbuff->xres = var.xres;
	ptDispbuff->yres = var.yres;
	ptDispbuff->bpp = var.bits_per_pixel;
	ptDispbuff->buf = (char *)fb_base;
	return 0;
}

static int fb_flushregion(p_region pt_region,pDispbuff ptDispbuff)
{
	return 0;
}



void FramebufferInit(void)
{
	RegisterDisplay(&g_tFramebufferOpr);
}
