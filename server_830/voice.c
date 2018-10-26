#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>     //open
#include <termios.h>
#include <unistd.h>   //read
#include <iconv.h>
#include"voice.h"
#define OUTLEN 256
#define MAIN_BUFMAX 36

//打开串口
int open_voiceport(void)
{
	int fd;
	fd=open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NONBLOCK);
	if(-1==fd)
          {
	perror("Can't Open SerialPort");
		}		
	if(fcntl(fd, F_SETFL, 0)<0) 
		printf("fcntl failed!\n"); 
        else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));    
	if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
	else 
		printf("isatty success!\n"); 
        printf("fd-open=%d\n",fd);
	return fd;
}
//初始化设置串口
int setting_voice(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
     struct termios newtio,oldtio; 	 	 
/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/ 
/*调用函数tcgetattr来初始化一个终端对应的termios结构
这个函数调用把当前终端接口变量的值写入termios_p参数指向的结构。
如果这些值其后被修改了，你可以通过调用函数tcsetattr来重新配置*/
     if  ( tcgetattr( fd,&oldtio)  !=  0) {  
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
void CloseSerial_voice(int fd)
{
	close(fd);
	printf("Serial is close\n");
}

void saysomething(char *say) 
{
	int fd;
	char temp = 0X00;
        fd=open_voiceport();    //打开串口   
        int nSpeed=9600;   //波特率
        int nBits=7;	   //设置停止位	
        char nEvent='o';    //设置奇偶校验位 
        int nStop=1;        //设置停止位
setting_voice(fd,nSpeed, nBits,nEvent,nStop);
 //待发送文本
unsigned char voice_encodeData[256]={0xFD,0x00,0x54,0x01,0x00};
//char *in_utf8 = "如果幸福是一粒渺小的种子那么感恩的心便是土壤自信犹如水源而无私则为阳光将幸福的种子植根于感恩的土壤用自信之泉浇灌用无私之光照耀我们看到幸福的大树茁壮成长郁郁苍苍";

char *in_utf8;
in_utf8=say;
unsigned char out[OUTLEN];
int rc;
rc = u2g(in_utf8,strlen(in_utf8),out,OUTLEN);//utf8码转为gb2312码
memcpy(voice_encodeData+5,out,OUTLEN);//将数组o复制到前数组v尾部
 //发送数据
 while(1)
 {
		if(( 0 > write(fd,voice_encodeData, 256)))
		{
			perror("Write is Error");
		//	return 0;
		}
    //           printf("temp = %d\n", temp);
		sleep(3);
		
		//temp++;

	//	if(11 == temp)
			break;
  }
	
	CloseSerial_voice(fd); //关闭串口
//	return 0;
}

//代码转换:从一种编码转为另一种编码
int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
iconv_t cd;
char **pin = &inbuf;
char **pout = &outbuf;
cd = iconv_open(to_charset,from_charset);
if (cd==0) 
return -1;
memset(outbuf,0,outlen);
if (iconv(cd,pin,&inlen,pout,&outlen)==-1) 
return -1;
iconv_close(cd);
return 0;
}
//UTF-8码转为GB2312码
int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

