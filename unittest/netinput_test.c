#include "input_manager.h"
#include "netinput.h"
#include <stdio.h>


int main(int argc, char **argv)
{
	InputEvent event;
	int ret;

	g_tNetinput.Device_Init();

	while(1)
		{
			ret = g_tNetinput.GetInputEvent(&event);
			if (ret) {
				printf("get inpout event error\n");
				return -1;
			}
			else
			{
				printf("type      : %d \n",event.iType);
				printf("str : %s \n",event.str);
			}
			
		}
	return 0;	
}


