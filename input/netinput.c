#include "input_manager.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_PORT 8888

static int g_iSocketServer;


static int NetinputGetInputEvent(pInputEvent ptInputEvent)
{

	struct sockaddr_in tSocketClientAddr;
	int iRecvLen;
	char ucRecvBuf[1000];
	socklen_t iAddrLen;

	iAddrLen = sizeof(struct sockaddr);
	iRecvLen = recvfrom(g_iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if (iRecvLen > 0)
	{
		ucRecvBuf[iRecvLen] = '\0';
		printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), ucRecvBuf);
		ptInputEvent->iType = INPUT_TYPE_NET;
		gettimeofday(&ptInputEvent->ttimeval,NULL);
		strncpy(ptInputEvent->str,ucRecvBuf,1000);
		ptInputEvent->str[999] = '\0';
		return 0;
			
	}
	else
	{
		return -1;
	}
	


	return 0;
}

static int NetinputDevice_Init(void)
{


	struct sockaddr_in tSocketServerAddr;
	int iRet;
	
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == g_iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}

	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}

	return 0;
}

static int NetinputDevice_Exit(void)
{
	close(g_iSocketServer);
	return 0;
}

InputDevice g_tNetinput = {
	.name = "Netinput",
	.GetInputEvent = NetinputGetInputEvent,
	.Device_Init = NetinputDevice_Init,
	.Device_Exit = NetinputDevice_Exit,
};

void NetinputDevRegiseter(void)
{
	regiseter_input_device(&g_tNetinput);
};
