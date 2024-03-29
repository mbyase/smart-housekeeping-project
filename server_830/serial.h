/*************************************************************************
	> File Name: serial.c
	> Author: Xianwen Wang
	> Mail: wangxianwenup@outlook.com 
	> Created Time: 2018年08月30日 星期四 17时33分41秒
 ************************************************************************/
#ifndef __SERIAL_H_
#define __SERIAL_H_


//void trans2(double B)；
int open_port(void);

int setting(int fd,int nSpeed, int nBits, char nEvent, int nStop);
void CloseSerial(int fd);

void init_serial();
void CloseSerial(int fd);
 void serial_start(int s);
void init_serial();

//void openfan();
 int  openfan();
 int closefan();
 int openbuzz();
int closebuzz();
int closelight();
int openlight();
 

#endif
 

/*
0x00 打开led1
0x01 关闭led1
0x02 打开蜂鸣器
0x03 关闭蜂鸣器
0x04 打开风扇
0x05 风扇低档
0x06 风扇中档
0x07 风扇高档
0x08 关闭风扇
0x09 打开7段数码管
0x10 关闭7段数码管
*/

