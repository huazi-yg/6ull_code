#include <stdio.h>
#include <string.h>
#include <tslib.h>
#include "input_manager.h"
#include "touchscreen.h"

static struct tsdev *g_ts;

static int TouchscreenGetInputEvent(pInputEvent ptInputEvent)
{
	struct ts_sample samp;
	int ret;

	ret = ts_read(g_ts, &samp, 1);

	if(ret != 1)
		return -1;
	
	ptInputEvent->iType = INPUT_TYPE_TOUCH;
	ptInputEvent->iX = samp.x;
	ptInputEvent->iY = samp.y;
	ptInputEvent->iPressure = samp.pressure;
	ptInputEvent->ttimeval = samp.tv;

	return 0;
}

static int TouchscreenDevice_Init(void)
{

	g_ts = ts_setup(NULL, 0);
	if (!g_ts)
	{
		printf("ts_setup err\n");
		return -1;
	}

	return 0;
}
static int TouchscreenDevice_Exit(void)
{
	ts_close(g_ts);
	return 0;
}

InputDevice g_tTouchscreen = {
	.name = "touchscreen",
	.GetInputEvent = TouchscreenGetInputEvent,
	.Device_Init = TouchscreenDevice_Init,
	.Device_Exit = TouchscreenDevice_Exit,
};
