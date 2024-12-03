#include "input_manager.h"
#include "touchscreen.h"


int main(int argc, char **argv)
{
	InputEvent event;
	int ret;

	input_init();//初始化输入
	input_device_init();//初始化设备

	while(1)
	{
		ret = get_input_event(&event);
		if (ret) {
			printf("get input event error\n");
			return -1;
		}
		else
		{
			if(event.iType = INPUT_TYPE_TOUCH)
			{
				printf("type      : %d \n",event.iType);
				printf("x         : %d \n",event.iX);
				printf("y         : %d \n",event.iY);
				printf("iPressure : %ud \n",event.iPressure);
			}
			else if(event.iType = INPUT_TYPE_NET)
			{
				printf("type	  : %d \n",event.iType);
				printf("str       : %s \n",event.str);
			}
			
		}
		
	}
	return 0;	
}


