#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	int fd;
	int status = 0;
//判断参数
	if(argc != 2)
	{
		printf("Usage :%s <dev> \n,",argv[0]);
		printf("    eg:%s /dev/button \n,",argv[0]);
		return -1;
	}
	//打开文件
	fd = open(argv[1],O_RDWR);
	if(fd < 0)
	{
		printf("can not open %s\n",argv[1]);
		return -1;
	}

	//写文件或这个读文件
	read(fd,&status,1);
	close(fd);
	return 0;
}
