

#ifndef _VOICE_H
#define _VOICE_H
// 用宏定义来代替全局变量

// #define OS "Windows 7"

//#define WEB_URL "http://www.baidu.com"
//#define WEB_NAME "百度"
// 也可以省略 extern，不过为了程序可读性，建议都写上
  extern int open_voiceport(void);
  
  int setting_voice(int fd,int nSpeed, int nBits, char nEvent, int nStop) ;
  void CloseSerial_voice(int fd);
  void saysomething(char *say);
  int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen);
int u2g(char *inbuf,int inlen,char *outbuf,int outlen);


 // extern long sum(int, int);
 // extern long mult(int, int);
 // extern char* getWebName();
 // extern char* getWebURL();
#endif
