# uart
  1. 设置串口总时钟
     UART_CLK_SEL设置串口的总时钟
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/27e6b7b7-0721-4137-84fc-7ab12e604e88)
     CSCDR1寄存器
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/b6189b81-e3fc-4c72-9a1c-299e2e10d17b)
    其中UART_CLK_SEL配置uart的时钟源，UART_CLK_ PODF配置uast分频系数
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/aaa1c192-88dd-4939-82cc-b97088bd3fc9)

  3. 设置串口的时钟
     使能串口的时钟
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/709377bf-225b-46aa-9a62-8c57964c739d)

  4. 配置引脚功能
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/cde37468-ff82-4b74-a66f-20e59a002bb6)
     ![image](https://github.com/huazi-yg/6ull_code/assets/62890683/8fa7a9ce-3dac-46fa-bc3d-9e2930e20511)


  6. 设置6ull特定的功能
     
  7. 设置波特率
      
  8. 设置数据格式
      
  9. IMX6ULL必须设置的参数

  ## 段的概念

  1. 代码段：RO CODE：程序本身，不会被修改
  2. 可读可写段：RW-DATA，可读可写段，全局变量，静态变量
  3. 只读段：RO DATA；可以放在ROM上，无需赋值到内存
  4. BSS或者ZI段
      - 初始值为0的全局变量和静态变量
      - 未初始化的全局变量和静态变量
  5. 局部变量，保存在栈中
  6. 一块空闲空间，堆，程序malloc函数分配

  ### 重定位
  把ROM中全局变量的在使用前赋值到内存中，叫数据重定位
  把ROM移动到其他位置，叫代码重定位。

  ### 重定位的作用


  ### RO-DATA DATA代码重定位

  
