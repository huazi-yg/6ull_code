
void delay(volatile unsigned int d)
{
	while(d--);
}


int  main()
{
	volatile unsigned int *pReg;

	//使能时钟CCM

	//配置GPIO复用功能
	pReg =(volatile unsigned int *) 0x2290014;
	*pReg |= 0x05;

	//设置GPIO为输出模式
	pReg =(volatile unsigned int *) 0x20AC004;
	*pReg |= 1<<3;
	
	pReg =(volatile unsigned int *) 0x20AC000;


	while(1)
	{
		*pReg |= (1<<3);
		delay(1000000);
		*pReg &= ~(1<<3);
		delay(1000000);
	}
					
	return 0;
}


