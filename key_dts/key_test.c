#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>

//#define BLOCK
int fd;

static void sig_func(int sig)
{
	int val = 0;
	read(fd,&val,4);
	printf("get button : 0x%x",val);
}

int main(int argc,char **argv)
{

	int val = 0;

	struct pollfd fds[1];
	int timeout_ms = 5000;
	int ret;
	int	flags;
	//判断参数
	if(argc != 2)
	{
		printf("Usage :%s <dev> \n,",argv[0]);
		return -1;
	}
	//注册信号函数
	signal(SIGIO,sig_func);
	//打开文件
	#if BLOCK
	fd = open(argv[1],O_RDWR);
	#else
	fd = open(argv[1],O_RDWR|O_NONBLOCK);
	#endif
	if(fd < 0)
	{
		printf("can not open %s\n",argv[1]);
		return -1;
	}

	//把进程ID传递给驱动
	//fcntl(fd,F_SETOWN,getpid());

	//使能驱动FASYNC
	flags = fcntl(fd,F_GETFL);
	fcntl(fd,F_SETFL,flags );

#if BLOCK
	fcntl(fd,F_SETFL,flags & ~ O_NONBLOCK);
#else
	fcntl(fd,F_SETFL,flags | O_NONBLOCK);
#endif

	while(1)
	{
		if(read(fd,&val,4) == 4)
		{
			printf("my_gpio_key 0x%x\n\n",val);
		}
		else
		{
			printf("while get button: -1\n");
		}
	}
	close(fd);
	return 0;
}
