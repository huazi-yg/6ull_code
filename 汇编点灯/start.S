
.text
.global	_start
_start:
;设置时钟

;设置IOMUX
	ldr r0, =(0x2290000+0x14)
	ldr r1,[r0]
	orr r1,r1,#0x05;
	str r1,[r0]
;设置输出方向
	ldr r0, = (0x20AC000+0x04)
	ldr r1,r[0]
	orr r1,r1,#(1<<3)
	str r1,[r0]
	b loop
	
	
loop:
	 ldr r2, = 0x20AC000
	 ldr r1,[r2]
	 orr r1 r1,#(1<<3)
	 str r1,[r2]
	 
	 ldr r0, = 1000000
	bl delay
		 ldr r2, = 0x20AC000
	 ldr r1,[r2]
	 bic r1 r1,#(1<<3)
	 str r1,[r2]
	ldr r0, = 1000000
	bl delay
	
	b loop

delay:




//设置输出方向

//设置电平状态


