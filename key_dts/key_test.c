#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <signal.h>


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
	fd = open(argv[1],O_RDWR);
	if(fd < 0)
	{
		printf("can not open %s\n",argv[1]);
		return -1;
	}

	//把进程ID传递给驱动
	fcntl(fd,F_SETOWN,getpid());

	//使能驱动FASYNC
	flags = fcntl(fd,F_GETFL);
	fcntl(fd,F_SETFL,flags | FASYNC);


	while(1)
	{
		printf("my_gpio_key\n");
		sleep(2);
	}
	close(fd);
	return 0;
}
