/*************************************************************************
	> File Name: serial.c
	> Author: Xianwen Wang
	> Mail: wangxianwenup@outlook.com 
	> Created Time: 2018年08月30日 星期四 17时33分41秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>     //open
#include <termios.h>
#include <unistd.h>   //read函数
#include "serial.h"
#define MAIN_BUFMAX 36



 
 	int fd;
	unsigned char buf[36] = {0};
	int len = 36;
	int m = 0;
	unsigned char *p=buf;
//	int *p = buf; 
    int i = 0;
	char temp = 0X00;
	
	
	

//打开串口
int open_port(void)
{
	//int fd;
					
	fd=open("/dev/ttyUSB1",O_RDWR | O_NOCTTY | O_NONBLOCK);
		//O_NONBLOCK设置为非阻塞模式，在read时不会阻塞住，在读的时候将read放在while循环中，下一节篇文档将详细讲解阻塞和非阻塞
							//	printf("fd=%d\n",fd);
								
	if(-1==fd)
		{
			perror("Can't Open SerialPort");
		}
	
	  // *恢复串口为阻塞状态
	 if(fcntl(fd, F_SETFL, 0)<0) 
		printf("fcntl failed!\n"); 
     else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0)); 
  // *测试是否为终端设备
	 if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
   else 
		printf("isatty success!\n"); 
   printf("fd-open=%d\n",fd);


	return fd;
	}



//初始化设置串口
int setting(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
     struct termios newtio,oldtio; 
	 /*最小的termios结构的典型定义如下：
	 
	 struct termios
	{
           tcflag_t c_iflag;
           tcflag_t c_oflag;
           tcflag_t c_cflag;
           tcflag_t c_lflag;
           cc_t           c_cc[NCCS];
	};
	上面五个结构成员名称分别代表：

    c_iflag：输入模式
    c_oflag：输出模式
    c_cflag：控制模式
    c_lflag：本地模式
    c_cc[NCCS]：特殊控制模式
	 
	 */
	 
	 
	 
	 
/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/ 

/*调用函数tcgetattr来初始化一个终端对应的termios结构
这个函数调用把当前终端接口变量的值写入termios_p参数指向的结构。
如果这些值其后被修改了，你可以通过调用函数tcsetattr来重新配置 
*/
     if ( tcgetattr( fd,&oldtio)  !=  0) {  
      perror("SetupSerial 1");
	printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio)); 
      return -1; 
     } 
     bzero( &newtio, sizeof( newtio ) ); 
/*步骤一，设置字符大小*/ 
     newtio.c_cflag  |=  CLOCAL | CREAD;  
     newtio.c_cflag &= ~CSIZE;  
/*设置停止位*/ 
     switch( nBits ) 
     { 
     case 7: 
      newtio.c_cflag |= CS7; 
      break; 
     case 8: 
      newtio.c_cflag |= CS8; 
      break; 
     } 
/*设置奇偶校验位*/ 
     switch( nEvent ) 
     { 
     case 'o':
     case 'O': //奇数 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag |= PARODD; 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      break; 
     case 'e':
     case 'E': //偶数 
      newtio.c_iflag |= (INPCK | ISTRIP); 
      newtio.c_cflag |= PARENB; 
      newtio.c_cflag &= ~PARODD; 
      break;
     case 'n':
     case 'N':  //无奇偶校验位 
      newtio.c_cflag &= ~PARENB; 
      break;
     default:
      break;
     } 
     /*设置波特率   cfsetispeed和cfsetospeed用来设置输入输出的波特率*/ 
     switch( nSpeed ) 
     { 
     case 2400: 
      cfsetispeed(&newtio, B2400); 
      cfsetospeed(&newtio, B2400); 
      break; 
     case 4800: 
      cfsetispeed(&newtio, B4800); 
      cfsetospeed(&newtio, B4800); 
      break; 
     case 9600: 
      cfsetispeed(&newtio, B9600); 
      cfsetospeed(&newtio, B9600); 
      break; 
     case 115200: 
      cfsetispeed(&newtio, B115200); 
      cfsetospeed(&newtio, B115200); 
      break; 
     case 460800: 
      cfsetispeed(&newtio, B460800); 
      cfsetospeed(&newtio, B460800); 
      break; 
     default: 
      cfsetispeed(&newtio, B9600); 
      cfsetospeed(&newtio, B9600); 
     break; 
     } 
/*设置停止位*/ 
     if( nStop == 1 ) 
      newtio.c_cflag &=  ~CSTOPB; 
     else if ( nStop == 2 ) 
      newtio.c_cflag |=  CSTOPB; 
/*设置等待时间和最小接收字符*/ 
     newtio.c_cc[VTIME]  = 0; 
     newtio.c_cc[VMIN] = 0; 
/*处理未接收字符  tcflush用于清空中端未完成的输入/输出请求及数据*/ 
     tcflush(fd,TCIFLUSH); 
/*激活新配置   调用函数tcsetattr来重新配置。*/ 
if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
     { 
      perror("com set error"); 
      return -1; 
     } 
     printf(" setting done!\n"); 
     return 0; 
} 

//关闭串口
void CloseSerial(int fd)
{
	close(fd);
	printf("Serial is close\n");
}



/*
//发送数据
int Sending(int fd, void *p, int data_len, int timeout)
{
	static struct pollfd fds;
	fds.fd    = fd;
	fds.events= POLLOUT;	
	fds.revents=0;	

	//60 second timeout
	int ret = poll(&fds, 1, timeout);
	if(0 >= ret)
	{
		perror("[write_serial]poll");
		return -1;	//error or timout/
	}

	if(fds.revents & POLLOUT)
		return write(fd, p, data_len);

	return -1;		//unknow error
	return write(fd, p, data_len);
}

*/


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

//unsigned char send[36] = {0X02,0XDD, 0X08, 0X04,0x07};
  unsigned char open_light[36] = {0xdd, 0x07, 0x24, 0x00, 0x00};
  unsigned char close_light[36] = {0xdd, 0x07, 0x24, 0x00, 0x01};
  unsigned char open_buzz[36] = {0xdd, 0x07, 0x24, 0x00, 0x02};
  unsigned char close_buzz[36] = {0xdd, 0x07, 0x24, 0x00, 0x03};
  unsigned char open_fan[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
  unsigned char close_fan[36] = {0xdd, 0x07, 0x24, 0x00, 0x08};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 // unsigned char send[36] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 
 
 void init_serial()
 {

	fd=open_port();      //打开串口
	int nSpeed=115200;  //波特率
	int nBits=7;		//设置停止位
	char nEvent='o';      //设置奇偶校验位
	int nStop=1;        //设置停止位
//setting(int fd,int nSpeed, int nBits, char nEvent, int nStop);
	setting(fd,nSpeed,nBits,nEvent,nStop);
 
	 
	 
	 
 }
 /*
 void openlight()
 {
	 
	 
	 if(( 0 > write(fd,open_light, 36)))
		{
			perror("Write is Error");
			return 0;
		}
		printf("temp = %d\n", temp);
		sleep(3);
		
		temp++;
	//	send[4] = temp;

		if(11 == temp)
			break;
 }
*/ 
 
int  openfan()
 {
	 init_serial();
	 
	/* if(( 0 > write(fd,open_fan, 36)))
		{
			perror("Write is Error");
		//	return 0;
		}*/
	int te=0;
	 write(fd,open_fan,7);
	printf("temp = %d\n", te);
	//	sleep(3);
		
	//	te++;
	//	send[4] = temp;

	//	if(11 == te)
	//	{

	     //    break;
	//	CloseSerial();
	   close(fd);

	//	te=0;
	    //    break;
	//	 }
        return 1;
 }
 
int  closefan()
 {
	 init_serial();
	 
	/* if(( 0 > write(fd,open_fan, 36)))
		{
			perror("Write is Error");
		//	return 0;
		}*/
	int te=0;
	write(fd,close_fan,7);
	printf("temp = %d\n", te);
	//	sleep(3);
		
	//	te++;
	//	send[4] = temp;

	//	if(11 == te)
	//	{

	//	}break;
	//	CloseSerial();
	   close(fd);

		te=0;
	//	break;
//		 }
return 1;
 }

openbuzz()
{
	init_serial();
	write(fd,open_buzz,7);
	close(fd);
	return 1;
}
closebuzz()
{
	init_serial();
	write(fd,close_buzz,7);
	close(fd);
	return 1;
}
openlight()
{
	init_serial();
	write(fd,open_light,7);
	close(fd);
	return 1;
}
closelight()
{
	init_serial();
	write(fd,close_light,7);
	close(fd);
	return 1;
}

 void serial_start(int s)
 {
	
	
 /*fd=open_port();      //打开串口
 int nSpeed=115200;  //波特率
 int nBits=7;		//设置停止位
 char nEvent='o';      //设置奇偶校验位
 int nStop=1;        //设置停止位
//setting(int fd,int nSpeed, int nBits, char nEvent, int nStop);
 setting(fd,nSpeed,nBits,nEvent,nStop);
*/

while(1)
	{
			 int i=0;
			 int id=0;
			 int len=0;
			 int temperature=0 ;    //温度
			 int light=0;					//光照
			 int humidity=0;       //湿度
	  	 memset(buf, 0, 36);  //清空buf
	 	 len = 36;
	   // *********	p = buf; 
		 while(len)
		 {
			 int num = read(fd, p, 36);
			 len -= num;
			 p   += num;
		 }
	

		//打印接受到的所有字符
		for(i=0;i<36;i++)
		{
		printf("%x  ",buf[i]);
		}
		printf("\n\n");
		if(buf[0] == 0x3b)
		{
		 for(i = 0; i<36; i++)
		 printf("%.2x ", buf[i]);
		
		 id=buf[1];
		 len=buf[2];
		 temperature = buf[4] +buf[5] ;
		 humidity=buf[7];
		 light=buf[20]+buf[21]*16*16+buf[22];

		 printf("id=%d\n",id);
		 printf("len=%d\n",len);
		 printf("temperature =%d\n",temperature );
		 printf("light=%d\n",light);
		}
		printf("\n");
		
		m++;
		printf("m = %d\n", m);
		
		if(5 == m)
			break;
	}

	//    char buf[10];
	//	buf=send[0];
	//	write(fd,buf,36)
	
    while(1)
	{
		if(1==s)   //开灯
		{
		  if(( 0 > write(fd,open_light, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		  printf("\nopen the light");
		  printf("temp = %d\n", temp);
		  sleep(3);
		  temp++;
	//	send[4] = temp;

		  if(11 == temp)
		  {
			  temp=0;
			break;
		  }
		}
		else if (2==s)  //关灯
		{
			if(( 0 > write(fd,close_light, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		  printf("temp = %d\n", temp);
		  sleep(3);
		
		  temp++;
	
		  if(11 == temp)
			 {
			  temp=0;
			break;
		  }
			
		}
		else if (3==s)  //开蜂鸣器
		{
			if(( 0 > write(fd,open_buzz, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		   printf("temp = %d\n", temp);
		   sleep(3);
		
		   temp++;
	
		  if(11 == temp)
			 {
			  temp=0;
			 break;
		     }
			
		}
		
		else if (4==s)  //关蜂鸣器
		{
			if(( 0 > write(fd,close_buzz, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		   printf("temp = %d\n", temp);
		   sleep(3);
		
		   temp++;
	
		  if(11 == temp)
			 {
			  temp=0;
			 break;
		     }
			
		}
		
		else if (5==s)  //开风扇
		{
			if(( 0 > write(fd,open_fan, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		   printf("temp = %d\n", temp);
		   sleep(3);
		
		   temp++;
	
		  if(11 == temp)
			 {
			  temp=0;
			 break;
		     }
			
		}
		
		else if (6==s)  //关风扇
		{
			if(( 0 > write(fd,close_fan, 36)))
		   {
			perror("Write is Error");
		//	return 0;
		   }
		   printf("temp = %d\n", temp);
		   sleep(3);
		
		   temp++;
	
		  if(11 == temp)
			 {
			  temp=0;
			 break;
		     }
		  int nu=0;
		  nu++;
		  if(50==nu)
		  {
		    nu=0;
			break;
		  }
			
		}
		
		else
		{
			
			printf("\n");
		}
	}
	
	
	// void closing()
	// {
	
	// CloseSerial(fd); //关闭串口
	
	// }
	


}
 
	 
	 
	 
 
 
 
  // int main()
 
// {
	// int fd;
	// unsigned char buf[36] = {0};
	// int len = 36;
	// int m = 0;
	// int *p = buf; 
    // int i = 0;
	// char temp = 0X00;


 // fd=open_port();      //打开串口
 // int nSpeed=115200;  //波特率
 // int nBits=7;		//设置停止位
 // char nEvent='o';      //设置奇偶校验位
 // int nStop=1;        //设置停止位
//setting(int fd,int nSpeed, int nBits, char nEvent, int nStop);
 // setting(fd,nSpeed,nBits,nEvent,nStop);


// while(1)
	// {
			// int i=0;
			// int id=0;
			// int len=0;
			// int temperature=0 ;    //温度
			// int light=0;					//光照
			// int humidity=0;       //湿度
	  	// memset(buf, 0, 36);  //清空buf
		// len = 36;
//	*********	p = buf; 
		// while(len)
		// {
			// int num = read(fd, p, 36);
			// len -= num;
			// p   += num;
		// }
	

//		打印接受到的所有字符
		// for(i=0;i<36;i++)
		// {
		// printf("%x  ",buf[i]);
		// }
		// printf("\n\n");
		// if(buf[0] == 0x3b)
		// {
		 // for(i = 0; i<36; i++)
		 // printf("%.2x ", buf[i]);
		
		 // id=buf[1];
		 // len=buf[2];
		 // temperature = buf[4] +buf[5] ;
     // humidity=buf[7];
		 // light=buf[20]+buf[21]*16*16+buf[22];

		 // printf("id=%d\n",id);
		 // printf("len=%d\n",len);
		 // printf("temperature =%d\n",temperature );
		 // printf("light=%d\n",light);
		// }
		// printf("\n");
		
		// m++;
		// printf("m = %d\n", m);
		
		// if(5 == m)
			// break;
	// }

	//   char buf[10];
	//	buf=send[0];
	//	write(fd,buf,36)
	
    // while(1)
	// {
		// if(( 0 > write(fd,open_light, 36)))
		// {
			// perror("Write is Error");
			// return 0;
		// }
		// printf("temp = %d\n", temp);
		// sleep(3);
		
		// temp++;
	//	send[4] = temp;
//serial_start(1); 
		// if(11 == temp)
			// break;
	// }
	
	// CloseSerial(fd); //关闭串口
	// return 0;


// }
