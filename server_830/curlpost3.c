#include <stdio.h>
#include <curl/curl.h>
#include "cJSON.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include  <string.h>
//#include <stdbool.h>
#include "curlpost3.h"


 /*
int packet(char** json_buffer)
{
    cJSON* root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("tom"));
    cJSON_AddItemToObject(root, "age", cJSON_CreateNumber(18));
 
    cJSON* score = cJSON_CreateArray();
    cJSON_AddItemToArray(score, cJSON_CreateNumber(91));
    cJSON_AddItemToArray(score, cJSON_CreateNumber(100));
    cJSON_AddItemToArray(score, cJSON_CreateNumber(78));
    cJSON_AddItemToArray(score, cJSON_CreateNumber(85));
 
    cJSON_AddItemToObject(root, "score", score);
    cJSON_AddItemToObject(root, "mobile", cJSON_CreateNull());
 
    char* buffer = cJSON_Print(root);
    cJSON_Delete(root);
    if(strlen(buffer)>1024)
    {
        printf("memsace too small, need enlarge json_buffer\n");
    }
    memcpy(*json_buffer,buffer,strlen(buffer));
    free(buffer);
    return 0;
}
*/

//extern cJSON *cJSON_CreateObject(void); 
//extern void cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);
 
//extern cJSON *cJSON_CreateNull(void); 
//extern cJSON *cJSON_CreateTrue(void); 
//extern cJSON *cJSON_CreateFalse(void);
//extern cJSON *cJSON_CreateBool(int b);
//extern cJSON *cJSON_CreateNumber(double num); 
//extern cJSON *cJSON_CreateString(const char *string);
//extern cJSON *cJSON_CreateArray(void);
//extern cJSON *cJSON_CreateObject(void);



char *out;
char *out2;
char * packet(char* json_buffer)
{
	/*
  // 创建JSON Object
  cJSON *root = cJSON_CreateObject();
  // 加入节点（键值对），节点名称为value，节点值为123.4
  cJSON_AddNumberToObject(root,"value",123.4);
  // 打印JSON数据包
  char *out = cJSON_Print(root);
  printf("%s\n",out);
  // 释放内存
  cJSON_Delete(root);
  free(out);
*/


    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();

	cJSON * next1 = cJSON_CreateObject();

	cJSON * next31 =cJSON_CreateObject();
	cJSON * next32 =cJSON_CreateObject();
	cJSON * next41 =cJSON_CreateObject();

    cJSON_AddItemToObject(root, "reqType",cJSON_CreateNumber(0));//根节点下添加
	
	cJSON_AddItemToObject(root,"perception",item);
	cJSON_AddItemToObject(item, "inputText", next);//semantic节点下添加item节点
	cJSON_AddItemToObject(next, "text", cJSON_CreateString(json_buffer)); //添text节点
	
	
//	cJSON_AddItemToObject(item, "inputImage", next1);//perception节点下添加inputImage节点
//	cJSON_AddItemToObject(next1, "url", cJSON_CreateString("imageUrl"));   //添url节
	
	cJSON_AddItemToObject(item, "selfInfo", next31);    //perception节点下添加inputImage节点
	cJSON_AddItemToObject(next31,"location",next32);   //添节
	cJSON_AddItemToObject(next32,"city",cJSON_CreateString("昆明"));   //添节
	cJSON_AddItemToObject(next32,"province",cJSON_CreateString("云南省"));   //添节
	cJSON_AddItemToObject(next32,"street",cJSON_CreateString("景明南路"));   //添节
	
	
	cJSON_AddItemToObject(root, "userInfo",next41);//根节点下添加
	cJSON_AddItemToObject(next41,"apiKey",cJSON_CreateString("911c18ca76b34696a99761334bf1ed86"));   //添节
	cJSON_AddItemToObject(next41,"userId",cJSON_CreateString("317709")); 
	
    
   printf("%s\n", cJSON_Print(root));
    
 //  char *out = cJSON_Print(root);
   out=cJSON_Print(root);
   printf("out=%s\n",out);
  // 释放内存
  cJSON_Delete(root);
  //  free(out);
   
  
   
   
   return out;
//	free(out);


}
	



/*
 
void unpacket(char* json_buffer)
{
    cJSON* root = cJSON_Parse(json_buffer);
    cJSON* score = cJSON_GetObjectItem(root, "score");
    cJSON* s2 = cJSON_GetArrayItem(score, 1);
 
    printf("s2 is %d\n", s2->valueint);
 
    // ?username=aa&password=bb
    // aa|bb
    // strtok
}
*/ 
 char * unpacket(char*  json_buffer)
{
	printf("开始解析！start unpacket \n");	
	cJSON *root_parse=cJSON_Parse(json_buffer);
	if(!root_parse)
	{

		printf("get root faild !\n");
	//	return -1;
	}
//	printf("%s\n",cJSON_Printf(root_parse));
	printf("\n****************\njson_buffer=%s\n",json_buffer);
//	printf("\n\n\n%s********************\nroot_parse=",root_parse);


//	char *pr=NULL;
//	pr=cJSON_Print(cJSON_GetArrayItem(root_parse,1));
//	if(!pr)
//	{
//		printf("no pr\n");
	//	return -1;
//	}
	cJSON *js_list = cJSON_GetObjectItem(root_parse, "results");
    
	if(!js_list)
	{
		printf("no list!\n");
//		return -1;
	}
	int array_size = cJSON_GetArraySize(js_list);
	cJSON *item=cJSON_GetArrayItem(js_list,0);

	cJSON *js_list2 = cJSON_GetObjectItem(item, "values");

	cJSON *js_list3 = cJSON_GetObjectItem(js_list2, "text");
	printf("item type is %d\n",js_list3->type);

	printf("text is:\n%s\n",js_list3->valuestring);
	json_buffer=js_list->valuestring;


//	cJSON *re=	cJSON_Print(js_list3);
    char *re;
	re=cJSON_Print(js_list3);
	printf("%s\n",re);
	printf("\n******************\n\n\n");

//	char reply=js_list3;
//	printf("%s \n",reply);
	if(root_parse)
	cJSON_Delete(root_parse);
//	return 0;
   out2=re;
  return re;
	
} 
 
 
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    char* buf = (char*)malloc(size*nmemb+1);
    buf[size*nmemb] = '\0';
    memcpy(buf, ptr, (size*nmemb));
    printf("recv data is: %s", buf);
    printf("\n************************************\n");
	unpacket(buf);
    free(buf);
    return (size * nmemb);
}
 
 
char* question(char *ask)
{
	CURL *curl;
	
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
    if(code != CURLE_OK)
    {
        printf("global init error\n");
      //  return 1;
    }
 
    char* data = (char*)malloc(1024);   //1024
//    packet(&data);
  //  char buf[]="今天天气怎么样";
      int len=strlen(ask);
  //    char buf[len];	//buf[512]
     char buf[256];
	 // buf=ask;
	//  buf=ask;
//	  printf("%s\n",buf);
	  char* s=packet(ask);

	  printf("s=%s\n",s);
	  ask=out;

	  printf("ask=%s\n",ask);

//	 sprintf(data,"{\"reqType\":0,\"perception\":{\"inputText\":{\"text\":\"%s\"},\"selfInfo\":{\"location\":{\"city\":\"昆明\",\"province\":\"云南\",\"street\":\"景明南路\"}}},\"userInfo\":{\"apiKey\":\"911c18ca76b34696a99761334bf1ed86\",\"userId\":\"317709\"}}",buf);

//	 printf("data=%s\n",data);

    curl = curl_easy_init();
	if(curl)
	 {

 
    // set option: URL
    curl_easy_setopt(curl, CURLOPT_URL, "http://openapi.tuling123.com/openapi/api/v2");
  //  curl_easy_setopt(curl, CURLOPT_PORT, 8081);
 
    // set option: Callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
 
    // set option: post method
    curl_easy_setopt(curl, CURLOPT_POST, 1);
 
    // 如果是post文本数据可以不设置长度，但是如果是二进制数据，需要指定长度。
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, out);   //data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(out)+1);
 
//    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "this is post data");//文本传输就不需要指定长度。
 
    // CURLOPT_POSTFIELDSIZE is option, if data is text
  //  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, );
 
    code = curl_easy_perform(curl);
    if(code != CURLE_OK)
    {
        printf("perform error\n");
    //    return 2;
    }
 }
    curl_easy_cleanup(curl);
	return out2;  //re
}
