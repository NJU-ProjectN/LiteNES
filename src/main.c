/*
litenes来自开源项目mynes
原作者：Stanislav Yaglo
项目地址：https://github.com/yaglo/mynes

这个版本的对mynes做了适当的重构和修改：
  为了保证顺利移植，所有程序对外部库的依赖都被减到最少。
  除了少量的文件引用string.h和一些常用的内存复制函数(如memcpy)，以及hal.c
  引用了allegro库外，其他代码均不涉及任何外部库和系统调用。

模拟器运行流程：
  1) 从参数中读取文件名(argv[1])
  2) 将文件读入到rom数组中
  3) 调用fce_load_rom(rom)分析ROM文件，判断ROM是否支持
  4) 调用fce_init()初始化模拟器
  5) 调用fce_run()进入模拟器循环
*/

#include "nes/fce.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static char rom[1048576];

void do_exit() // normal exit at SIGINT
{
	exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: mynes romfile.nes\n");
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Open rom file failed.\n");
        exit(1);
    }
    int nread = fread(rom, sizeof(rom), 1, fp);
    if (nread == 0 && ferror(fp)) {
        fprintf(stderr, "Read rom file failed.\n");
        exit(1);
    }
    if (fce_load_rom(rom) != 0) {
        fprintf(stderr, "Invalid or unsupported rom.\n");
        exit(1);
    }
	signal(SIGINT, do_exit);
    fce_init();
    fce_run();
    return 0;
}
