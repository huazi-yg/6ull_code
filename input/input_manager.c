#include <stdio.h>
#include <string.h>
#include <tslib.h>
#include "input_manager.h"
#include "touchscreen.h"
#include "netinput.h"
#include "pthread.h"
#include <unistd.h>
#include <semaphore.h>

static pInputDevice g_InputDevs = NULL;

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;

/* 实现环形缓冲区 begin */

#define BUF_SIZE 20
static int g_iRead;
static int g_iWrite;
static InputEvent g_tInputEvents[BUF_SIZE];

static int isInputBufferFull(void)
{
	return (g_iRead == ((g_iWrite + 1) % BUF_SIZE));
}

static int isInputBufferEmpty(void)
{
	return (g_iRead == g_iWrite);
}

static void PutInputEvent(pInputEvent ptInputEvent)
{
	if(!isInputBufferFull())
	{
		g_tInputEvents[g_iWrite] = *ptInputEvent;
		g_iWrite = (g_iWrite + 1) % BUF_SIZE;
	}
}

static int GetInputEventFromBuffer(pInputEvent ptInputEvent)
{
	if(!isInputBufferEmpty())
	{
		*ptInputEvent = g_tInputEvents[g_iRead];
		g_iRead = (g_iRead + 1) % BUF_SIZE;
		return 1;
	}
	else
		return 0;
}

/* 实现环形缓冲区 end */

void regiseter_input_device(pInputDevice ptInputEvent)
{
	ptInputEvent->ptNext = g_InputDevs;
	g_InputDevs = ptInputEvent;
}

static void *input_recv_thread_func(void *data)
{
	pInputDevice t_inputdev = (pInputDevice)data;
	InputEvent t_event;
	int ret;

	while(1)
	{
		/* 读数据 */
		ret = t_inputdev->GetInputEvent(&t_event);

		/* 保存数据 */
		if(!ret)
		{
			pthread_mutex_lock(&g_tMutex);
			PutInputEvent(&t_event);
			
			/* 唤醒等待数据的线程 */
			pthread_cond_signal(&g_tConVar);
			pthread_mutex_unlock(&g_tMutex);
		}
		
	}
	return 0;
}
void input_init(void)
{
	/* regiseter touchscreen*/
	TouchscreenDevRegiseter();

	/* regiseter netinput*/ 
	NetinputDevRegiseter();
}

void input_device_init(void)
{
	int ret;
	pInputDevice ptTemp;
	pthread_t tid;
	/* init each input device,create thread*/
	
	ptTemp = g_InputDevs;
	while(ptTemp)
	{
		//init device
		ret = ptTemp->Device_Init();	
		//pthread*/
		if(!ret)
		{
			pthread_create(&tid,NULL,input_recv_thread_func,ptTemp);
		}
		ptTemp = ptTemp->ptNext;
	}
}

int get_input_event(pInputEvent ptInputEvent)
{
	InputEvent tEvent;
	int ret;
	/* 无数据，休眠 */
	pthread_mutex_lock(&g_tMutex);
	if(GetInputEventFromBuffer(&tEvent))
	{
		*ptInputEvent = tEvent;
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	else
	{
		pthread_cond_wait(&g_tConVar,&g_tMutex);
		if(GetInputEventFromBuffer(&tEvent))
		{
			*ptInputEvent = tEvent;
			ret = 0;
		}
		else
		{
			ret = -1;
		}
		pthread_mutex_unlock(&g_tMutex);
	}
	return ret;
	/* 返回数据 */
}
