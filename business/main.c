#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "i2cbusses.h"


/*
 * ./test <iic_bus number> w "i2c hello"
 * ./test 0 r
 */
int main(int argc, char **argv)
{
	unsigned char dev_addr = 0x50;
	unsigned char mem_addr = 0;
	unsigned char buf[32];
	int file;
	char filename[20];
	char *str;

	if(argc != 3 && argc != 4)
	{
		printf("Usage: \n");
		printf("write eeprom:%s <dev> w str\n",argv[0]);
		printf("read  eeprom:%s <dev> r\n",argv[0]);
		return -1;
	}

	file = open_i2c_dev((int)(argv[1][0]-'0'),filename,sizeof(filename),0);
	if(file < 0)
	{
		printf("can not open %s\n",filename);
		return -1;
	}

	
	if(set_slave_addr(file,dev_addr,1))
	{
		printf("can not set_slave_addr \n");
		return -1;
	}

	if (argv[2][0] == 'w')
	{
		//write
		str = argv[3];
		while(*str)
		{
			//mem_addr ,*str
			i2c_smbus_write_byte_data(file,mem_addr,*str);
			mem_addr++;str++;
		}
		i2c_smbus_write_byte_data(file,mem_addr,0);
	}
	else
	{
		i2c_smbus_read_i2c_block_data(file,mem_addr,sizeof(buf),buf);
		buf[31] = '\0';
		printf("get data:%s\n",buf);
	}

	return 0;	
}


