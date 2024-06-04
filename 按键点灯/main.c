
void delay(volatile unsigned int d)
{
	while(d--);
}


int  main()
{
	/* gpio begin*/
	volatile unsigned int *pReg;
	volatile unsigned int *pReg1;
	//使能时钟CCM,GPIO5默认使能

	//配置GPIO复用功能
	pReg =(volatile unsigned int *) 0x2290014;
	*pReg |= 0x05;

	//设置GPIO为输出模式
	pReg =(volatile unsigned int *) 0x20AC004;
	*pReg |= 1<<3;
	
	pReg =(volatile unsigned int *) 0x20AC000;

	pReg1 = (volatile unsigned int *) 0x20C4074;
	*pReg1 |= 3 << 12;

	//配置GPIO复用功能
	pReg1 = (volatile unsigned int *) 0x20E01B0;
	*pReg1 &= ~0x0F;
	*pReg1 |= 5;

	//配置GPIO为输入模式
	pReg1 = (volatile unsigned int *) 0x20A8004;
	*pReg1 |= 0<<14;

	pReg1 = (volatile unsigned int *) 0x20A8000;

	/* gpio end*/
		*pReg |= (1<<3);
		delay(1000000);
		*pReg &= ~(1<<3);
		delay(1000000);
		*pReg |= (1<<3);	
	while(1)
	{

		if((*pReg1 & (1<<14)) == 0)
		{
			*pReg |= (1<<3);
		}
		else
		{
			*pReg &= ~(1<<3);
		}
	}
					
	return 0;
}


