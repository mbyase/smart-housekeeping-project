/*************************************************************************
	> File Name: server_process.c
	> Author: Xianwen Wang
	> Mail: wangxianwenup@outlook.com 
	> Created Time: 2018年08月29日 星期三 18时53分14秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "time.h"
#include <string.h>
#include <pthread.h>
#include "curlpost3.h" 
#include <stdbool.h>
#include "voice.h"
#include"serial.h"
#include"labour.h"

#define FALSE 0
#define TRUE 1
static char bufimage[1024*1024];

void docmd2(char* cmd,int sfd)
{
	int i = 0;
	int ret = -1;
	if(0==strcmp(cmd,"pic"))
	{
		int fd = open("image_jpg.jpg",O_RDONLY);
		int len = 0;
		len = 0;
		while(1)
		{
			int ret = 0;
			ret = read(fd,bufimage+len,1024);
			if(ret<=0)
			{
				break;
			}
			len = len + ret;
		}
		printf("len is %d\n",len);
		char rbuf[20]={0};
		sprintf(rbuf,"%d",len);
		ret = write(sfd,&len,20);
		printf("len is %d\n",ret);
		ret = 0;
		while(ret < len)
		{
			int tmp = write(sfd,bufimage+ret,len);
			ret = ret + tmp;
		}
		printf("len is %d\n",ret);
	}
	else
	{
		printf("error cmd\n");
	}
}
 
openingfan()
{
// init_serial();
 //int fd=open_port();
// init_serial();
int fd=open("/dev/ttyUSB1",O_RDWR | O_NOCTTY | O_NONBLOCK);
//init_serial();
 setting(fd,115200,7,'o',1);

 unsigned char open_fan[7] = {0xdd, 0x07, 0x24, 0x00, 0x04};
 write(fd,open_fan,7);
 close(fd);
 }

 //用来专门传送图片 该服务器端口8888
void thread(void)
{
	int i;
	for(i=0;i<3;i++)
	{

		printf("This is a pthread.\n");
		
	}	
	//第一步.创建SOCKET套接字
    
    //第一个参数AF_INET表示使用IPv4协议族
    //第二个参数SOCK_STREAM表示使用TCP传输协议，SOCK_DGRAM表示使用UDP传输协议
    //第三个参数0表示使用协议族中默认协议
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == listenfd )
    {
        perror("socket");
    //    return -1;
    }

    //第二步.绑定IP地址和端口号

    struct sockaddr_in serveraddr = {0};
    //使用IPv4协议族
    serveraddr.sin_family = AF_INET;
    //端口号转换为网络字节序
    serveraddr.sin_port = htons(8888);
    //IP地址转换为网络字节序
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.2.127");
	
	/*INADDR_ANY 转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP*/
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定IP地址和端口号
    int ret = bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if( -1 == ret )
    {
        perror("bind");
     //   return -1;
    }

    //第三步.监听

    //第一个参数指明要监听的套接字
    //第二个参数指明全连接队列的大小
    ret = listen(listenfd,10);  //5
    if( -1 == ret )
    {
        perror("listen");
    //    return -1;
	}
    //第四步.建立连接
    
    //第一个参数指明监听套接字
    //第二个参数为保存客户端IP地址和端口号的结构体指针
    //第三个参数表示结构体大小的指针
    struct sockaddr_in clientaddr = {0};
    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
    if( -1 == connfd )
    {
        perror("accept");
    //    return -1;
    }
   
    printf("连接建立成功!（子服务器）\n");
	printf("Connected successfully!(sub-server)\n");
//	openingfan();
    //数据收发
    
	//	 write(connfd,"王先文123abc",25);
    
	
//	 if(FALSE==init_v4l2()) 
//		{
//	    	printf("init_v412==0\n");

	//	return (0);
							         
//		}
//	  v4l2_grab();   //捕捉

	FILE * fp2;
	if(0==init_v4l2()) 

        {
	printf("\ninit error\n");
      //  return(0);

        }
 	v4l2_grab();

        char buf1[32];


	memset(buf1,0,32);

	ret = read(connfd,buf1,32);



	 while(1)

	{


	   deal();
	   docmd(buf1,connfd);

	    printf("hello");
		
	

/*
	
//  struct sockaddr_in c_addr;
	 memset(&clientaddr, 0, sizeof(clientaddr)); //memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法 
    int len = sizeof(clientaddr);
	int connfd = accept(listenfd, (struct sockaddr *) &clientaddr,&len);
	printf("accept ok\n");	
	while(1)
		{
//			deal();   //

			//dothings
			char buf[32];
			int ret = 0;
			memset(buf,0,32);
			ret = read(connfd,buf,32);
			if(0==ret)
			
			{
				break;
			}
			docmd(buf,connfd);
		}
		//关闭
		close(connfd);

 
	 }
	close(listenfd);
//	 close_v4l2();
*/

	}

        printf("save JPEG OK\n");

        close(connfd);

		close(listenfd);

        close_v4l2();


}



void thread2(void)
{
	int i;
	for(i=0;i<3;i++)
	{

		printf("This is a pthread2.\n");
		
	}	
	//第一步.创建SOCKET套接字
    
    //第一个参数AF_INET表示使用IPv4协议族
    //第二个参数SOCK_STREAM表示使用TCP传输协议，SOCK_DGRAM表示使用UDP传输协议
    //第三个参数0表示使用协议族中默认协议
    int listenfd2 = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == listenfd2 )
    {
        perror("socket");
    //    return -1;
    }

 //第二步.绑定IP地址和端口号

    struct sockaddr_in serveraddr2 = {0};
    //使用IPv4协议族
    serveraddr2.sin_family = AF_INET;
    //端口号转换为网络字节序
    serveraddr2.sin_port = htons(7777);
    //IP地址转换为网络字节序
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.2.127");
	
	/*INADDR_ANY 转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP*/
    serveraddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定IP地址和端口号
    int ret2 = bind(listenfd2,(struct sockaddr *)&serveraddr2,sizeof(serveraddr2));
    if( -1 == ret2 )
    {
        perror("bind");
     //   return -1;
    }

    //第三步.监听

    //第一个参数指明要监听的套接字
    //第二个参数指明全连接队列的大小
    ret2 = listen(listenfd2,10);  //5
    if( -1 == ret2 )
    {
        perror("listen");
    //    return -1;
	}

	
    //第四步.建立连接
    
    //第一个参数指明监听套接字
    //第二个参数为保存客户端IP地址和端口号的结构体指针
    //第三个参数表示结构体大小的指针
    struct sockaddr_in clientaddr2 = {0};
    socklen_t addrlen2 = sizeof(clientaddr2);
    int connfd2 = accept(listenfd2,(struct sockaddr *)&clientaddr2,&addrlen2);
    if( -1 == connfd2 )
    {
        perror("accept");
    //    return -1;
    }
   
    printf("连接建立成功!（子服务器2）\n");
	printf("\n Connected successfully!(sub-2-server)\n");
//	openingfan();
    //数据收发
    
	//	 write(connfd,"王先文123abc",25);
    
//	FILE * fp2;
//	 if(FALSE==init_v4l2()) 
//		{
//	    	printf("init_v412==0\n");

	//	return (0);
							         
//		}


 while(1)

	{

//		init_serial();

	//  struct sockaddr_in c_addr;
      memset(&clientaddr2, 0, sizeof(clientaddr2)); //memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法 
    int len2 = sizeof(clientaddr2);
//	int connfd2 = accept(listenfd2, (struct sockaddr2 *) &clientaddr2,&len2);
	printf("accept ok\n");	
	while(1)
		{


			/*dothings*/
			char buf2[100]={0};
			char que2[100]={0};
			int ret3 = 0;
			memset(buf2,0,sizeof(buf2));
			ret3 = read(connfd2,buf2,sizeof(buf2));
			printf("buf is %s\n",buf2);
		//	if(0==ret)
			
	//		{
	//			break;
	//		}

	//		init_serial();

     if((0==strcmp("turn on the light",buf2))|(0==strcmp("开灯",buf2))|(0==strcmp("打开灯",buf2)))

		{
		int flag=openlight();
		if(1==flag)
		{
			printf("open light successfully\n");
		}
		else
		{
			printf("\nopen light error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经开灯\n");
			
			continue;
			//saysomething("开灯了");
			//serial_start(1);  //开灯
			//发送命令到串口里面，去控制单片机的灯
			//char a1[]={"已经开灯"};
			//saysomething(a1);
			
			
		}
	else if((0==strcmp("turn off the light",buf2))|(0==strcmp("关灯",buf2))|(0==strcmp("关闭灯",buf2)))

		{
		int flag=closelight();
		if(1==flag)
		{
			printf("close light successfully\n");
		}
		else
		{
			printf("\nclose light error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经关灯\n");
			
			continue;
			//saysomething("开灯了");
			//serial_start(1);  //开灯
			//发送命令到串口里面，去控制单片机的灯
			//char a1[]={"已经开灯"};
			//saysomething(a1);
		//	openfan();
			//serial_start(2);  //关灯
			//发送命令到串口里面，去控制单片机的灯
			//char a2[]={"已经关灯"};
			//saysomething(a2);

		}

     else if((0==strcmp("turn on the buzz",buf2))|(0==strcmp("开蜂鸣器",buf2))|(0==strcmp("打开蜂鸣器",buf2)))

		{
		int flag=openbuzz();
		if(1==flag)
		{
			printf("open buzz successfully\n");
		}
		else
		{
			printf("\nopen buzz error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经打开蜂鸣器\n");
			
			continue;

//serial_start(3);  //开蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
		//	char a3[]={"已经打开蜂鸣器"};
		//	saysomething(a3);

		}
  else if((0==strcmp("turn off the buzz",buf2))|(0==strcmp("关蜂鸣器",buf2))|(0==strcmp("关闭蜂鸣器",buf2)))

		{
		int flag=closebuzz();
		if(1==flag)
		{
			printf("close buzz successfully\n");
		}
		else
		{
			printf("\nclose buzz error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经关闭蜂鸣器\n");
			
			continue;

//serial_start(3);  //开蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
		//	char a3[]={"已经打开蜂鸣器"};
		//	saysomething(a3);
			//serial_start(4);  //关蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
			//char a4[]={"已经关蜂鸣器"};
			//saysomething(a4);

		}
// else if((0==strcmp("2",buf))|(0==strcmp("开风扇",buf))|(0==strcmp("打开风扇",buf)))
 else if((0==strcmp("turn on the fan",buf2)))

		{
	//		openingfan();
		int flag=	openfan();
		if(1==flag)
		{
			printf("open fan successfully\n");
		}
		else
		{
			printf("\nopen fan error");
		}
		//	init_serial();
		//	serial_start(5);  //开风扇
		//	saysomething("开风扇了\n");
			//发送命令到串口里面，去控制单片机的灯
			char a5[]={"已经打开风扇"};
			saysomething(a5);
			//break;
			continue;
		}
  else if((0==strcmp("turn off the fan",buf2))|(0==strcmp("关风扇",buf2))|(0==strcmp("关闭风扇",buf2)))

		{
		int flag=closefan();
		if(1==flag)
		   {
			printf("open fan successfully\n");
		   }
		else
		   {
			printf("\nopen fan error");
		   }
			//serial_start(6);  //关风扇
			//发送命令到串口里面，去控制单片机的灯
			char a6[]={"已经关闭风扇了"};
			saysomething(a6);
			continue;


		}

/*	else
		{
		//把buf里面的内容发送到图灵，将返回的结果又存在buf里面  
		//	strcpy(buf,	send_turing(buf));
		strcpy(que2,buf2);
	//	char p[100]={0};
	//	*p=question(buf);

	//	printf("p=%s",p);
           char * p2=question(que2);	
	//	printf("");
		printf("\n**********************************\n");
		printf("question is :%s\n",que2);
	    saysomething(p2); //使用语音模块来发声
//		strcpy(buf,que);
		write(connfd2,p2,strlen(p2));

	//	write(connfd,que,strlen(que));  //返回客户端对话的内容
		memset(buf2,0,sizeof(buf2));
		memset(que2,0,sizeof(que2));
				
		}
*/


	//		docmd(buf2,connfd2);
		}
		/*关闭*/
	//	close(connfd2);

 
	 }
	close(listenfd2);
//	 close_v4l2();

}

  int main(void)
{
	pthread_t id,id2;
	int i,ret_xiancheng,j,ret_xiancheng2;
	//函数pthread_create用来创建一个线程
	// 第一个参数为指向线程标识符的指针，第二个参数用来设置线程属性
	// 第三个参数是线程运行函数的起始地址，最后一个参数是运行函数的参数。
	ret_xiancheng=pthread_create(&id,NULL,(void *) thread,NULL); // 成功返回0，错误返回错误编号
		if(ret_xiancheng!=0)
			{
				printf ("Create pthread error!\n");
		//	    exit (1);
			}
		for(i=0;i<3;i++)
		{
			printf("This is the main process.\n");
		}
		//函数pthread_join用来等待一个线程的结束
	//	pthread_jon(id,NULL);      //多了这一句就连不上主服务器


   ret_xiancheng2=pthread_create(&id2,NULL,(void *) thread2,NULL); // 成功返回0，错误返回错误编号
		if(ret_xiancheng2!=0)
			{
				printf ("Create pthread2 error!\n");
		//	    exit (1);
			}
		
		//函数pthread_join用来等待一个线程的结束
	//	pthread_jon(id,NULL);      //多了这一句就连不上主服务器




		
	//主服务器端口9999 用于传送数据以及控制命令
	//第一步.创建SOCKET套接字
    
    //第一个参数AF_INET表示使用IPv4协议族
    //第二个参数SOCK_STREAM表示使用TCP传输协议，SOCK_DGRAM表示使用UDP传输协议
    //第三个参数0表示使用协议族中默认协议
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == listenfd )
    {
        perror("socket");
        return -1;
    }

    //第二步.绑定IP地址和端口号

    struct sockaddr_in serveraddr = {0};
    //使用IPv4协议族
    serveraddr.sin_family = AF_INET;
    //端口号转换为网络字节序
    serveraddr.sin_port = htons(9999);
    //IP地址转换为网络字节序
   // serveraddr.sin_addr.s_addr = inet_addr("192.168.43.237");
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定IP地址和端口号
    int ret = bind(listenfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if( -1 == ret )
    {
        perror("bind");
        return -1;
    }

    //第三步.监听

    //第一个参数指明要监听的套接字
    //第二个参数指明全连接队列的大小
    ret = listen(listenfd,10);
    if( -1 == ret )
    {
        perror("listen");
        return -1;
	}
    //第四步.建立连接
    
    //第一个参数指明监听套接字
    //第二个参数为保存客户端IP地址和端口号的结构体指针
    //第三个参数表示结构体大小的指针
    struct sockaddr_in clientaddr = {0};
    socklen_t addrlen = sizeof(clientaddr);
		int connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&addrlen);
		if( -1 == connfd )
    {
        perror("accept");
        return -1;
    }
   
    printf("连接建立成功!（主服务器）\n");
	printf("Connected successfully!(main-server)\n");
    //数据收发
    
	//	 write(connfd,"王先文123abc",25);
    

 while(1)

{
	char buf[100]={0};
	
	char que[100]={0};
	srand((unsigned)time(NULL));
	int test=rand()%3;
	read(connfd,buf,sizeof(buf));
  //  read(connfd,buf,128);
	printf("buf=%s",buf);
	printf("\n");
															
	if((0==strcmp("你好",buf)))
																								 
	{
	//	memset(buf,0,sizeof(buf));
		srand( (unsigned)time( NULL ) );   /* 输出随机数*/  
																																							            
		int test=rand()%3;
		char *ans[]={"你好啊","你也好","好久不见"};
		strcpy(buf,ans[test]);
		saysomething(buf);
		write(connfd,ans[test],strlen(ans[test]));
		memset(buf,0,sizeof(buf));
	}

	else if((0==strcmp("turn on the light",buf))|(0==strcmp("开灯",buf))|(0==strcmp("打开灯",buf)))

		{
		//	saysomething("开灯了");
		//	serial_start(1);  //开灯
			//发送命令到串口里面，去控制单片机的灯
		//	char a1[]={"已经开灯"};
		//	saysomething(a1);
		int flag=openlight();
		if(1==flag)
		{
			printf("open light successfully\n");
		}
		else
		{
			printf("\nopen light error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经开灯\n");
			
			continue;
			//saysomething("开灯了");
			//serial_start(1);  //开灯
			//发送命令到串口里面，去控制单片机的灯
			//char a1[]={"已经开灯"};
			//saysomething(a1);

			
			
		}
	else if((0==strcmp("turn off the light",buf))|(0==strcmp("关灯",buf))|(0==strcmp("关闭灯",buf)))

		{
		//	serial_start(2);  //关灯
			//发送命令到串口里面，去控制单片机的灯
		//	char a2[]={"已经关灯"};
		//	saysomething(a2);
		int flag=closelight();
		if(1==flag)
		{
			printf("close light successfully\n");
		}
		else
		{
			printf("\nclose light error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经关灯\n");
			
			continue;
			//saysomething("开灯了");
			//serial_start(1);  //开灯
			//发送命令到串口里面，去控制单片机的灯
			//char a1[]={"已经开灯"};
			//saysomething(a1);
		//	openfan();
			//serial_start(2);  //关灯
			//发送命令到串口里面，去控制单片机的灯
			//char a2[]={"已经关灯"};
			//saysomething(a2);


		}
	else if((0==strcmp("turn on the buzz",buf))|(0==strcmp("开蜂鸣器",buf))|(0==strcmp("打开蜂鸣器",buf)))

		{
			//serial_start(3);  //开蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
			//char a3[]={"已经打开蜂鸣器"};
			//saysomething(a3);

		int flag=openbuzz();
		if(1==flag)
		{
			printf("open buzz successfully\n");
		}
		else
		{
			printf("\nopen buzz error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经打开蜂鸣器\n");
			
			continue;

//serial_start(3);  //开蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
		//	char a3[]={"已经打开蜂鸣器"};
		//	saysomething(a3);

		}
	else if((0==strcmp("turn off the buzz",buf))|(0==strcmp("关蜂鸣器",buf))|(0==strcmp("关闭蜂鸣器",buf)))

		{
			//serial_start(4);  //关蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
			//char a4[]={"已经关蜂鸣器"};
			//saysomething(a4);
		int flag=closebuzz();
		if(1==flag)
		{
			printf("close buzz successfully\n");
		}
		else
		{
			printf("\nclose buzz error");
		}
		
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开蜂鸣器"};
			saysomething("已经关闭蜂鸣器\n");
			
			continue;

//serial_start(3);  //开蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
		//	char a3[]={"已经打开蜂鸣器"};
		//	saysomething(a3);
			//serial_start(4);  //关蜂鸣器
			//发送命令到串口里面，去控制单片机的灯
			//char a4[]={"已经关蜂鸣器"};
			//saysomething(a4);

		}
	else if((0==strcmp("turn on the fan",buf))|(0==strcmp("开风扇",buf))|(0==strcmp("打开风扇",buf)))

		{
			//serial_start(5);  //开风扇
			//发送命令到串口里面，去控制单片机的灯
			//char a5[]={"已经打开风扇"};
			//saysomething(a5);
		int flag=	openfan();
		if(1==flag)
		{
			printf("open fan successfully\n");
		}
		else
		{
			printf("\nopen fan error");
		}
		//	init_serial();
		//	serial_start(5);  //开风扇
		//	saysomething("开风扇了\n");
			//发送命令到串口里面，去控制单片机的灯
			char a5[]={"已经打开风扇"};
			saysomething(a5);
			//break;
			continue;

		}
	else if((0==strcmp("turn off the fan",buf))|(0==strcmp("关风扇",buf))|(0==strcmp("关闭风扇",buf)))

		{
			//serial_start(6);  //关风扇
			//发送命令到串口里面，去控制单片机的灯
			//char a6[]={"已经打开风扇"};
			//saysomething(a6);
			int flag=closefan();
		if(1==flag)
		   {
			printf("open fan successfully\n");
		   }
		else
		   {
			printf("\nopen fan error");
		   }
			serial_start(6);  //关风扇
			//发送命令到串口里面，去控制单片机的灯
			char a6[]={"已经关闭风扇了"};
			saysomething(a6);
			continue;


		}

	else
		{
		//把buf里面的内容发送到图灵，将返回的结果又存在buf里面  
		//	strcpy(buf,	send_turing(buf));
		strcpy(que,buf);
	//	char p[100]={0};
	//	*p=question(buf);

	//	printf("p=%s",p);
char *	p=	question(que);	
	//	printf("");
		printf("\n**********************************\n");
		printf("question is :%s\n",que);
	    saysomething(p); //使用语音模块来发声
//		strcpy(buf,que);
		write(connfd,p,strlen(p));

	//	write(connfd,que,strlen(que));  //返回客户端对话的内容
		memset(buf,0,sizeof(buf));
		memset(que,0,sizeof(que));
				
		}
		
		
//  int fd;
//  int nread;
//  char buff[255];
//  char *dev_name = "/dev/ttyUSB0";//根据实际情况选择串口
//	 while(1) 
//		 {  
//		 fd = open(dev_name,O_RDWR | O_NOCTTY | O_NONBLOCK); //打开串口 
														 
//		 if(fd>0) 
//			 ;
	//	 set_speed(fd,115200); //设置波特率 
//		 else 
//			 { 
//				 printf("Can't Open Serial Port!\n"); 
//		     sleep(1);
//			 	 continue; 
//				  break;
//				;				  
	//	  }


	

		 }

return 0;


}
