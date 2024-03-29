  //  #include "labor.h"
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <sys/ioctl.h>
    #include <stdlib.h>
    #include <sys/mman.h>
    #include <linux/types.h>
    #include <linux/videodev2.h>
    #include <setjmp.h>
    #include "jpeglib.h"
    #include <string.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include"labour.h"

 //   typedef int BOOL;
     
    #define  TRUE   1
    #define  FALSE  0
    #define CLEAR(x) memset(&(x), 0, sizeof(x))//qingneicun
    #define FILE_VIDEO     "/dev/video0"
    
    #define YUV        "image_yuv.yuv"
    #define JPEG        "image_jpeg.jpg"
     
    #define  IMAGEWIDTH    320
    #define  IMAGEHEIGHT   240
     
    static   int      fd;
    static   struct   v4l2_capability   cap;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_format fmt,fmtack;
    struct v4l2_streamparm setfps;  
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;
    enum v4l2_buf_type type;
    unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];
     
    struct buffer
    {
        void * start;
        unsigned int length;
    } * buffers;

	static char bufimage[1024*1024];
//FILE *fp2;
	void docmd(char* cmd,int sfd)
	{
          
	int i = 0;
	int ret = -1;
        printf("%s\n",cmd);
	//if(0==strcmp(cmd,"pic"))
	//{
                
		int fd1 = open("image_jpeg.jpg",O_RDONLY);
                if (fd1 == -1)
                  {
                    printf("wrong");
                  }
		int len = 0;
		len = 0;
		while(1)
		{
			int ret = 0;
			ret = read(fd1,bufimage+len,1024);
			if(ret<=0)
			{
				break;
			}
			len = len + ret;
		}
		printf("len is %d\n",len);
		char rbuf[20]={0};
		sprintf(rbuf,"%d",len);
		ret = write(sfd,&rbuf,20);
		printf("len is %d\n",ret);
		ret = 0;
		while(ret < len)
		{
			int tmp = write(sfd,bufimage+ret,len);
			ret = ret + tmp;
		}
		printf("len is %d\n",ret);
	/*}
	else
	{
		printf("error cmd\n");
	}*/
                }
                
	
   int init_v4l2(void)
    {
        int i;
        int ret = 0;
        
        //opendev
        if ((fd = open(FILE_VIDEO, O_RDWR)) == -1) 
        {
            printf("Error opening V4L interface\n");
            return (FALSE);
        }
     
        //query cap
        if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) 
        {
            printf("Error opening device %s: unable to query device.\n",FILE_VIDEO);
            return (FALSE);
        }
        else
        {
             printf("driver:\t\t%s\n",cap.driver);
             printf("card:\t\t%s\n",cap.card);
             printf("bus_info:\t%s\n",cap.bus_info);
             printf("version:\t%d\n",cap.version);
             printf("capabilities:\t%x\n",cap.capabilities);
             
             if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) 
             {
                printf("Device %s: supports capture.\n",FILE_VIDEO);
            }
     
            if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) 
            {
                printf("Device %s: supports streaming.\n",FILE_VIDEO);
            }
        } 
        
        //emu all support fmt
        fmtdesc.index=0;
        fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        printf("Support format:\n");
        while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
        {
            printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
            fmtdesc.index++;
        }
        
        //set fmt
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.height = IMAGEHEIGHT;
        fmt.fmt.pix.width = IMAGEWIDTH;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
        
        if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
        {
            printf("Unable to set format\n");
            return FALSE;
        }     
        if(ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
        {
            printf("Unable to get format\n");
            return FALSE;
        } 
        {
             printf("fmt.type:\t\t%d\n",fmt.type);
             printf("pix.pixelformat:\t%c%c%c%c\n",fmt.fmt.pix.pixelformat & 0xFF, (fmt.fmt.pix.pixelformat >> 8) & 0xFF,(fmt.fmt.pix.pixelformat >> 16) & 0xFF, (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
             printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
             printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
             printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);
        }
        //set fps
        setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        setfps.parm.capture.timeperframe.numerator = 10;
        setfps.parm.capture.timeperframe.denominator = 10;
        
        printf("init %s \t[OK]\n",FILE_VIDEO);
            
        return TRUE;
    }
     
    int v4l2_grab(void)
    {
        unsigned int n_buffers;
        
        //request for 4 buffers 
        req.count=4;
        req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory=V4L2_MEMORY_MMAP;
        if(ioctl(fd,VIDIOC_REQBUFS,&req)==-1)
        {
            printf("request for buffers error\n");
        }
     
        //mmap for buffers
        buffers = malloc(req.count*sizeof (*buffers));
        if (!buffers) 
        {
            printf ("Out of memory\n");
            return(FALSE);
        }
        
        for (n_buffers = 0; n_buffers < req.count; n_buffers++) 
        {
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = n_buffers;
            //query buffers
            if (ioctl (fd, VIDIOC_QUERYBUF, &buf) == -1)
            {
                 printf("query buffer error\n");
				 return(FALSE);
            }
     
            buffers[n_buffers].length = buf.length;
            //map
            buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ |PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
            if (buffers[n_buffers].start == MAP_FAILED)
            {
                 printf("buffer map error\n");
                return(FALSE);
            }
        }
		
            
        //queue
        for (n_buffers = 0; n_buffers < req.count; n_buffers++)
        {
			//buf.type =V4L2_BUF_TYPE_VIDEO_CAPTURE;
		    //buf.memory =V4L2_MEMORY_MMAP;
            buf.index = n_buffers;
            ioctl(fd, VIDIOC_QBUF, &buf);
        } 
        
           type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl (fd, VIDIOC_STREAMON, &type) < 0)
	{

	    printf("VIDIOCSTREAMON error\n");
		return -1;
	 }
	}
     
     
  
    void yuvv_rgb(char * pointer, unsigned char * frame_buffer)
{
  int  i,j;
    unsigned char y1,y2,u,v;
    int r1,g1,b1,r2,g2,b2;
    //char * pointer;
    
	//pointer = buffers[0].start;
	
    for(i=0;i<IMAGEHEIGHT;i++)
    {
    	for(j=0;j<IMAGEWIDTH/2;j++)
    	{
    		y1 = *( pointer + (i*IMAGEWIDTH/2+j)*4);
    		u  = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 1);
    		y2 = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 2);
    		v  = *( pointer + (i*IMAGEWIDTH/2+j)*4 + 3);
    		
    		r1 = y1 + 1.042*(v-128);
    		g1 = y1 - 0.34414*(u-128) - 0.71414*(v-128);
    		b1 = y1 + 1.772*(u-128);
    		
    		r2 = y2 + 1.042*(v-128);
    		g2 = y2 - 0.34414*(u-128) - 0.71414*(v-128);
    		b2 = y2 + 1.772*(u-128);
    		
    		if(r1>255)
    			r1 = 255;
    		else if(r1<0)
    			r1 = 0;
    		
    		if(b1>255)
    			b1 = 255;
    		else if(b1<0)
    			b1 = 0;	
    		
    		if(g1>255)
    			g1 = 255;
    		else if(g1<0)
    			g1 = 0;	
    			
    		if(r2>255)
    			r2 = 255;
    		else if(r2<0)
    			r2 = 0;
    		
    		if(b2>255)
    			b2 = 255;
    		else if(b2<0)
    			b2 = 0;	
    		
    		if(g2>255)
    			g2 = 255;
    		else if(g2<0)
    			g2 = 0;		
    			
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6    ) = (unsigned char)b1;
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6 + 1) = (unsigned char)g1;
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6 + 2) = (unsigned char)r1;
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6 + 3) = (unsigned char)b2;
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6 + 4) = (unsigned char)g2;
    		*(frame_buffer + (i*IMAGEWIDTH/2+j)*6 + 5) = (unsigned char)r2;
    	}
    }
}
	
    int encode_jpeg(char *lpbuf,int width,int height)
    {
        struct jpeg_compress_struct cinfo ;
        struct jpeg_error_mgr jerr ;
        JSAMPROW  row_pointer[1] ;
        int row_stride ;
        char *buf=NULL ;
        int x ;
     
        FILE *fptr_jpg = fopen (JPEG,"wb");//注意这里为什么用fopen而不用open
        if(fptr_jpg==NULL)
        {
        printf("Encoder:open file failed!/n") ;
         return FALSE;
        }
     
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, fptr_jpg);
     
        cinfo.image_width = width;
        cinfo.image_height = height;
        cinfo.input_components = 3;
        cinfo.in_color_space = JCS_RGB;
     
        jpeg_set_defaults(&cinfo);
     
     
        jpeg_set_quality(&cinfo, 80,TRUE);
     
     
        jpeg_start_compress(&cinfo, TRUE);
     
        row_stride = width * 3;
        buf=malloc(row_stride) ;
        row_pointer[0] = buf;
        while (cinfo.next_scanline < height)
        {
         for (x = 0; x < row_stride; x+=3)
        {
     
        buf[x]   = lpbuf[x];
        buf[x+1] = lpbuf[x+1];
        buf[x+2] = lpbuf[x+2];
     
        }
        jpeg_write_scanlines (&cinfo, row_pointer, 1);//critical
        lpbuf += row_stride;
        }
     
        jpeg_finish_compress(&cinfo);
        fclose(fptr_jpg);
        jpeg_destroy_compress(&cinfo);
        free(buf) ;
        return TRUE ;
     
     }  
     void deal()
   {
	FILE* file_fd = fopen(YUV, "w");//yuyv图片
	if (file_fd < 0)
	{
		perror("open test_mmap.jpg fialed! \n");
	    exit(-1);		
	}
 
    CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	int ret = ioctl(fd, VIDIOC_DQBUF, &buf);//出列采集的帧缓冲,成功返回0
	if(0 != ret)
	{
		printf("VIDIOC_DQBUF failed!\n");
		exit(-1); 
	}    
	ret = fwrite(buffers[0].start, buffers[0].length, 1, file_fd);//将摄像头采集得到的yuyv数据写入文件中     
	if(ret <= 0)
	{
	    printf("write yuyv failed!\n");
	    exit(-1);
    }
  
    unsigned char rgb[IMAGEWIDTH*IMAGEHEIGHT*3];
    
    yuvv_rgb(buffers[0].start, rgb);
    FILE* pic = fopen("rgb","w");
    fwrite(rgb, sizeof(rgb),1, pic);
    //savejpg(rgb);
    encode_jpeg(rgb,IMAGEWIDTH,IMAGEHEIGHT);
    fclose(file_fd);
    fclose(pic);
    if(-1 == ioctl(fd,VIDIOC_QBUF, &buf))
    {
        printf("VIDIOC_QBUF failed!\n");
    }
   }
    int close_v4l2(void)
    {
         if(fd != -1) 
         {
             close(fd);
             return (TRUE);
         }
         return (0);
    }
     
  /*   
    int labour()
    {
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
    serveraddr.sin_port = htons(8888);
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
    

 
		
        FILE * fp2;
     
        if(0==init_v4l2()) 
        {
        return(0);
        }
		
        v4l2_grab();
     char buf1[32];
		      //  int ret = 0;
			memset(buf1,0,32);
			ret = read(connfd,buf1,32);
       while(1)
	   {
               deal();
		   //dothings
                       
			
			//if(0==ret)
			//{
				//break;
			//}
                       // connfd
			docmd(buf1,connfd);
		       printf("hello");
                        
		        
	   }
	 
        //yuyv_2_rgb888();                  //yuyv to RGB24
     
        encode_jpeg(frame_buffer,IMAGEWIDTH,IMAGEHEIGHT); //RGB24 to Jpeg
		
       printf("save "JPEG"OK\n");
        close(connfd);
	close(listenfd);
        close_v4l2();
     

    }*/
