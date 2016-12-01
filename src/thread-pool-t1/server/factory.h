#ifndef __FACTORY_H__
#define __FACTORY_H__

#include"queue.h"


typedef void* (*pfunc)(void*);
typedef struct
{
	pthread_t *pthid;
	int pthread_num;
	pfunc pthread_func;
	queue q;
	pthread_cond_t cond;
	int capability;
	int flag;
}factory,*pfactory;

void factory_init(pfactory);
void factory_start(pfactory);
void* pthread_server(void*);
void send_n(int,char *,int );
void send_file(int);
#endif
