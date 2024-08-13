# Makefile

## Makefile的规则

```bash
target:prerequisites
command
```
prerequisites比target修改时间新，就执行后面的command命令

## Makefile工作原理
1. 当执行`make`后,make会在当前目录下找makefile或者Makefile文件
2. 找到makefile文件后，会继续找第一个目标文件target，并把这个目标文件当作最终的生成文件
3. 如果target不存在或者prerequisites的修改时间比target新，就执行后面的命令生成target
4. prerequisites都存在，会在当前文件中找生成prerequisites的文件

## 变量
定义变量`var = file1 file2`
使用变量`$(var)`
当需要加入新的文件时，我们只需要往变量中var加入新的文件即可

## 自动推导

## 特殊符号
```bash
target:prerequisites
 $@ $^ $<

```
$@ $^ $<分别代表左边的内容，右边的内容和右边第一个内容
在方法前面加入@表示执行时不显示到终端
.PHONY 伪目标

## Makefile
主要包含：显示规则，隐晦规则，变量定义，文件指示和注释
### 文件指示
1. 在一个Makefile中引用另一个Makefile
2. 根据情况指定Makefile中有效的部分
3. 定义一个多行的命令

### 引用其他Makefile文件
   include filename
   include前面可以是空格，但是绝对不能有Tab。

   `include a.mk b.mk`

### 文件搜索


### 自动生成依赖
```bash
gcc -M main.c#生成
main.o: main.c defs.h /usr/include/stdio.h /usr/include/features.h \
    /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
    /usr/lib/gcc-lib/i486-suse-linux/2.95.3/include/stddef.h \
    /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
    /usr/include/bits/sched.h /usr/include/libio.h \
    /usr/include/_G_config.h /usr/include/wchar.h \
    /usr/include/bits/wchar.h /usr/include/gconv.h \
    /usr/lib/gcc-lib/i486-suse-linux/2.95.3/include/stdarg.h \
    /usr/include/bits/stdio_lim.h

```



[makefile 葵花宝典](https://blog.csdn.net/yredh_114/article/details/69389134)
[](https://sinzz.blog.csdn.net/article/details/133983534)