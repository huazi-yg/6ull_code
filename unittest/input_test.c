#include "input_manager.h"
#include "touchscreen.h"


int main(int argc, char **argv)
{
	InputEvent event;
	int ret;

	g_tTouchscreen.Device_Init();

	while(1)
		{
			g_tTouchscreen.GetInputEvent(&event);
			if (ret) {
				printf("get inpout event error\n");
				return -1;
			}
			else
			{
				printf("type      : %d \n",event.iType);
				printf("x         : %d \n",event.iX);
				printf("y         : %d \n",event.iY);
				printf("iPressure : %ud \n",event.iPressure);
			}
			
		}
	return 0;	
}


