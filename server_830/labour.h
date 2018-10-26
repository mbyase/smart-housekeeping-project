#ifndef __LABOUR_H__
#define __LABOUR_H__


	int init_v4l2(void);
	int v4l2_grab(void);
	void yuvv_rgb(char * pointer, unsigned char * frame_buffer);
    int  encode_jpeg(char *lpbuf,int width,int height);
	int close_v4l2(void);
	int process();

	
#endif
