#include <math.h>
#include <string.h>

#include "ui.h"
#include "page_manager.h"
#include "common.h"
#include "stdio.h"
#include "config.h"

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];
static int g_tButtonCnt;

static int main_onpressd(struct Button *ptButton,pDispbuff ptDispbuff,pInputEvent ptInputEvent)
{	

	unsigned int dw_color = BUTTON_DEFAULT_COLOR;
	char name[100];
	char status[100];
	char *strbutton;

	strbutton = ptButton->name;
	//对于触摸屏事件，默认颜色为红色
	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
	
		//1.1分辨能否被点击
		if(get_itemcfg_by_name(ptButton->name)->bCanbeTouched == 0)
		{
			return -1;
		}
		//1.2修改颜色
		if(ptInputEvent->iPressure >= 1)
		{
			ptButton->status = !ptButton->status;
		}
		printf("status %d str %ud \n",ptButton->status,ptInputEvent->iPressure);
		if(ptButton->status)
		{
			dw_color = BUTTON_PRESSED_COLOR;
		}
	}
	else if(ptInputEvent->iType == INPUT_TYPE_NET)//2对于网络类事件
	{	
		//根据传入的字符串改变颜色
		strbutton = ptButton->name;
		sscanf(ptInputEvent->str,"%s %s",name,status);
		if(strcmp(status,"ok") == 0)
		{
			dw_color = BUTTON_PRESSED_COLOR;
		}
		else if(strcmp(status,"err") == 0)
		{
			dw_color = BUTTON_DEFAULT_COLOR;
		}
		else if(status[0] > 0 && status[0] < 9)
		{
			dw_color = BUTTON_PERCENT_COLOR;
			strbutton = status;
		}
	}

	//绘制底色
	DrawRegion(&ptButton->t_region,dw_color);
	//增加文字

	DrawTextRegionCenter(strbutton,&ptButton->t_region,BUTTON_TEXT_COLOR);
	flushregion(&ptButton->t_region,ptDispbuff);

	return 0;
}


static void Generatebutton(void)
{
	int width,height;
	int n_per_line;
	int row,rows;
	int col;
	int n;
	pDispbuff ptDispbuff = GetDisplayBuffer();

	pButton ptButton;
	int i = 0;

	int xres,yres;
	int start_x,start_y;
	int pre_start_x,pre_start_y;
	//计算单个的按钮的width/height
	n = get_itemcfg_count();
	g_tButtonCnt = n;

	xres = ptDispbuff->xres;
	yres = ptDispbuff->yres;
	width = sqrt(1.0/0.618*xres*yres/n);

	n_per_line = xres /width + 1;
	width = xres/n_per_line;
	height = 0.55*width;

	

	//居中显示每个按钮的region
	start_x = (xres -width * n_per_line)/2;
	rows = n/n_per_line;
	if(rows * n_per_line < n)
	{
		rows += 1;
	}
	start_y = (yres - rows*height)/2;
	//计算每个按钮的region
	for(row = 0;(row < rows) && (i < n);row++)
	{
		pre_start_y = start_y + row*height;
		pre_start_x =  start_x - width;
		for(col = 0;(col < n_per_line) && (i < n);col++)
		{
			if(i == n){
				break;
			}
			printf("i \n");

			ptButton = &g_tButtons[i];
			ptButton->t_region.iLeftupx = pre_start_x + width;
			ptButton->t_region.ileftupy = pre_start_y;
			ptButton->t_region.iwidth = width - X_GAP;
			ptButton->t_region.iheigh = height - Y_GAP;
			pre_start_x = ptButton->t_region.iLeftupx;
			button_init(get_itemcfg_by_index(i)->name, ptButton, NULL,NULL,main_onpressd);
			
			i++;
		}
	}
	
	for(i=0;i<n;i++)
	{
		g_tButtons[i].OnDraw(&g_tButtons[i],ptDispbuff);
	}
}

static int isTouchPointInRegion(int iX,int iY,p_region pt_region)
{
	if(iX < pt_region->iLeftupx || iX > pt_region->iLeftupx + pt_region->iwidth)
	{
		return 0;
	}
	if(iY < pt_region->ileftupy || iY > pt_region->ileftupy + pt_region->iheigh)
	{
		return 0;
	}
	return 1;
}

static pButton get_button_by_name(char *name)
{
	int i;
	for (i = 0; i < g_tButtonCnt;i++)
	{
		if(strcmp(name,g_tButtons[i].name) == 0)
		{
			return &g_tButtons[i];
		}
	}

	return NULL;
}

static pButton get_button_by_input_event(pInputEvent ptInputEvent)
{
	int i;
	char name[100];
	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		for(i = 0;i<g_tButtonCnt;i++)
		{
			if(isTouchPointInRegion(ptInputEvent->iX,ptInputEvent->iY,&g_tButtons[i].t_region))
			{
				return &g_tButtons[i];
			}
		}
	}
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		sscanf(ptInputEvent->str,"%s",name);
		return get_button_by_name(name);
	}
	else 
	{
		return NULL;
	}
	return NULL;
}



static void MainPageRun(void *pParams)
{
	
	int error;
	InputEvent tInputEvent;
	pButton ptButton;
	pDispbuff ptDispbuff = GetDisplayBuffer();
	/* 读取配置文件 */
	error = parse_config_file();

	if(error)
	{
		return ;
	}
	/* 根据配置文件生产按钮，界面 */
	Generatebutton();
	
	while(1)
	{
		/* 读取输入事件 */
		error = get_input_event(&tInputEvent);
		if(error)
		{
			continue;
		}
		#if 0
		if((tInputEvent.iType = INPUT_TYPE_TOUCH))
		{
			printf("type      : %d \n",tInputEvent.iType);
			printf("x         : %d \n",tInputEvent.iX);
			printf("y         : %d \n",tInputEvent.iY);
			printf("iPressure : %ud \n",tInputEvent.iPressure);
		}
		else if((tInputEvent.iType = INPUT_TYPE_NET))
		{
			printf("type	  : %d \n",tInputEvent.iType);
			printf("str       : %s \n",tInputEvent.str);
		}		
		#endif
		/* 根据输入事件找到按钮 */
		ptButton = get_button_by_input_event(&tInputEvent);
		if(!ptButton)
		{
			continue;
		}
		/* 调用按钮的onpressed按键 */
		ptButton->onpressd(ptButton,ptDispbuff,&tInputEvent);
	}
}

static PageAction g_tMainPage = {
	.name = "main",
	.Run = MainPageRun,
		
};

void MainPageRegister(void)
{
	PageRegister(&g_tMainPage);
}


