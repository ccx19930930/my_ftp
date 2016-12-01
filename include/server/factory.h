#ifndef __FACTORY_H__
#define __FACTORY_H__

#include"head.h"
#include"conf.h"
#include"protocol.h"

typedef void* (*pfunc)(void*);


typedef struct
{
	pthread_t pthid;
	int fdw;
}main_pth,*pmain_pth;

typedef struct
{
	pconf_struct conf;
	int fdr;
}sub_pth,*psub_pth;

typedef struct
{
	pthread_t *pthid;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	file_queue_type f;
	pfunc pf;
}file_serve_struct;



void factory(pconf_struct conf);


#endif
