/*************************************************************************
	> File Name: tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月11日 星期三 09时25分06秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include "time.h"
#include <string.h>
int main()
{
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
    //serveraddr.sin_addr.s_addr = inet_addr("192.168.2.127");
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
    ret = listen(listenfd,5);
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
   
    printf("连接建立成功!\n");
    //数据收发
    
	//	 write(connfd,"王先文123abc",25);
    

		 while(1)

		 {
		 char buf[256];
     srand((unsigned)time(NULL));
		 int test=rand()%3;
		 read(connfd,buf,sizeof(buf));
     printf("buf=%s\n",buf);

		 if((0==strcmp("你好",buf)))
		 
		 {
		 memset(buf,0,sizeof(buf));
                 srand( (unsigned)time( NULL ) );   /* 输出随机数*/  
                 int test=rand()%3;
		 char *ans[]={"你好啊","你也好","好久不见"};
		 strcpy(buf,ans[test]);
		//  write(connfd,buf,strlen(buf));
		write(connfd,buf,strlen(buf));
		memset(buf,0,sizeof(buf));
		 }
		 
		 else if((0==strcmp("你是谁",buf)))
		 {
		 memset(buf,0,sizeof(buf));
		 srand((unsigned)time(NULL));
		 int test=rand()%3;
		 char *ans[]={"我是qt机器人","我是昆明理工大学，通信工程151的机器人","我当然是一堆代码"};
		 strcpy(buf,ans[test]);
		 write(connfd,buf,strlen(buf));
		 memset(buf,0,sizeof(buf));
		 }
		 
		 else if((0==strcmp("成都天气怎么样",buf)))
		 {
		 memset(buf,0,sizeof(buf));
		 srand((unsigned)time(NULL));
		 int test=rand()%3;
		 char *ans[]={"当然是很热啦","天气一般般，不太好","天气很热啊，想吃冰淇淋"};
		 strcpy(buf,ans[test]);
		 write(connfd,buf,strlen(buf));
		 memset(buf,0,sizeof(buf));
		 }
               
		 else if(0==strcmp("off",buf))
		 {
		 memset(buf,0,sizeof(buf));
		 write(connfd,"已经关闭风扇");
		 memset(buf,0,sizeof(buf));
		 }

		 else if(0==strcmp("on",buf))
		 {
		 memset(buf,0,sizeof(buf));
		 write(connfd,"已经打开风扇");
		 memset(buf,0,sizeof(buf));
		 }

		 else if(0==strcmp("turn on",buf))
		 {
		 memset(buf,0,sizeof(buf));
		 write(connfd,"已经开灯");
		 memset(buf,0,sizeof(buf));
		 }
		 
		 else if(0==strcmp("turn off",buf))
		 {
		  memset(buf,0,sizeof(buf));
		  write(connfd,"已经关灯");
		  memset(buf,0,sizeof(buf));
		 }
		  else if(0==strcmp("turn on the air conditioner",buf))
		 {
		  memset(buf,0,sizeof(buf));
		  write(connfd,"已经打开空调啦");
		  memset(buf,0,sizeof(buf));
		 }
		else if(0==strcmp("turn off air conditioner",buf))
		 {
		  memset(buf,0,sizeof(buf));
		  write(connfd,"已经关闭空调咯");
		  memset(buf,0,sizeof(buf));
		 }
		 else if(0==strcmp("turn on the tv",buf))
		 {
		  memset(buf,0,sizeof(buf));
		  write(connfd,"已经打开电视啦");
		  memset(buf,0,sizeof(buf));
		 }
		 else if(0==strcmp("turn off the tv",buf))
		 {
		  memset(buf,0,sizeof(buf));
		  write(connfd,"已经关闭电视啦");
		  memset(buf,0,sizeof(buf));
		 }
         
		  else if(0==strcmp("weather",buf))
		 {
		 memset(buf,0,sizeof(buf));
		 srand((unsigned)time(NULL));
		 int test=rand()%3;
		 char *ans[]={"成都市武侯区，天气晴朗，气温30摄氏度","武侯区，天气晴朗，气温30摄氏度，注意防晒哦","成都市，天气晴朗，气温30摄氏度，注意防晒哦"};
		 strcpy(buf,ans[test]);
		 write(connfd,buf,strlen(buf));
		 memset(buf,0,sizeof(buf));
			 
			 
		
		 }
		 
		 
		 
		 else 
		 {
		 memset(buf,0,sizeof(buf));
		 srand((unsigned)time(NULL));
		 int test=rand()%3;
		 char *ans[]={"这个问题我还不知道哦","不知道你在说什么","听不懂你说什么"};
		 strcpy(buf,ans[test]);
		 write(connfd,buf,strlen(buf));
		 memset(buf,0,sizeof(buf));
		 }
		 
		 }
                 
		 
    close(connfd);
    return 0;
}

