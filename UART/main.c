
void delay(volatile unsigned int d)
{
	while(d--);
}

/*根据IMX6ULL芯片手册<<55.15 UART Memory Map/Register Definition>>的3608页，定义UART的结构体,*/
typedef struct {
  volatile unsigned int  URXD;               /**< UART Receiver Register, offset: 0x0 	           串口接收寄存器，偏移地址0x0     */
  		   unsigned char RESERVED_0[60];		
  volatile unsigned int  UTXD;               /**< UART Transmitter Register, offset: 0x40          串口发送寄存器，偏移地址0x40*/
  		   unsigned char RESERVED_1[60];		
  volatile unsigned int  UCR1;               /**< UART Control Register 1, offset: 0x80 	       串口控制寄存器1，偏移地址0x80*/
  volatile unsigned int  UCR2;               /**< UART Control Register 2, offset: 0x84 	       串口控制寄存器2，偏移地址0x84*/
  volatile unsigned int  UCR3;               /**< UART Control Register 3, offset: 0x88            串口控制寄存器3，偏移地址0x88*/
  volatile unsigned int  UCR4;               /**< UART Control Register 4, offset: 0x8C            串口控制寄存器4，偏移地址0x8C*/
  volatile unsigned int  UFCR;               /**< UART FIFO Control Register, offset: 0x90         串口FIFO控制寄存器，偏移地址0x90*/
  volatile unsigned int  USR1;               /**< UART Status Register 1, offset: 0x94             串口状态寄存器1，偏移地址0x94*/
  volatile unsigned int  USR2;               /**< UART Status Register 2, offset: 0x98             串口状态寄存器2，偏移地址0x98*/
  volatile unsigned int  UESC;               /**< UART Escape Character Register, offset: 0x9C     串口转义字符寄存器，偏移地址0x9C*/
  volatile unsigned int  UTIM;               /**< UART Escape Timer Register, offset: 0xA0         串口转义定时器寄存器 偏移地址0xA0*/
  volatile unsigned int  UBIR;               /**< UART BRM Incremental Register, offset: 0xA4      串口二进制倍率增加寄存器 偏移地址0xA4*/
  volatile unsigned int  UBMR;               /**< UART BRM Modulator Register, offset: 0xA8 	   串口二进制倍率调节寄存器 偏移地址0xA8*/
  volatile unsigned int  UBRC;               /**< UART Baud Rate Count Register, offset: 0xAC      串口波特率计数寄存器 偏移地址0xAC*/
  volatile unsigned int  ONEMS;              /**< UART One Millisecond Register, offset: 0xB0      串口一毫秒寄存器 偏移地址0xB0*/
  volatile unsigned int  UTS;                /**< UART Test Register, offset: 0xB4                 串口测试寄存器 偏移地址0xB4*/		
  volatile unsigned int  UMCR;               /**< UART RS-485 Mode Control Register, offset: 0xB8  串口485模式控制寄存器 偏移地址0xB8*/
} UART_Type;

void uart_init(void)
{
		/* gpio begin*/
	volatile unsigned int *CCM_CSCDR1;
	volatile unsigned int *CCM_CCGR5;
	volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA;
	volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA;
	volatile unsigned int *IOMUXC_UART1_RX_DATA_SELECT_INPUT;

	UART_Type *uart = (UART_Type *)0x02020000;

	CCM_CSCDR1 = (volatile unsigned int *)0x020c4024;
	CCM_CCGR5 = (volatile unsigned int *)0x020c407C;
	IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = (volatile unsigned int *)0x020E0084;
	IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = (volatile unsigned int *)0x020E0088;
	IOMUXC_UART1_RX_DATA_SELECT_INPUT = (volatile unsigned int *)0x020E0624;

	*CCM_CSCDR1 &= ~((1 << 6) | 0x3F);
	*CCM_CCGR5 |= (3<<24);
	*IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA &= ~(0x0F);
	*IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA &= ~(0x0F);
	*IOMUXC_UART1_RX_DATA_SELECT_INPUT |= 0x03;

	//设置波特率
	uart->UFCR |= (5<<7);
	uart->UBIR = 15;
	uart->UBMR = 693;

	//设置数据格式
	uart->UCR2 |= (1 << 14) | (0 << 8) | (0 << 6) | (1 << 5) | (1 << 2) | (1 << 1) ;

	uart->UCR3 |= (1 << 2) ;
	//使能uart
	uart->UCR1 |= (1 << 0);

}

int getchar(void)
{
	UART_Type *uart = (UART_Type *)0x02020000;

	while((uart->USR2 & (1 << 0)) == 0);
	return uart->URXD;
}

void putchar(char c)
{
	UART_Type *uart = (UART_Type *)0x02020000;;

	while((uart->USR2 & (1 << 3)) == 0);
	uart->UTXD = c;
}

int  main()
{
	char c;
	uart_init();
	putchar('a');
	while(1)
	{
		c = getchar();
		putchar(c);
		putchar(c+1);
	}
					
	return 0;
}


