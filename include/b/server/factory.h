#ifndef __FACTORY_H__
#define __FACTORY_H__

typedef void* (*pfunc)(void*);

typedef struct factory_set
{
	pthread_t pthid[6];
	pfunc pth_fun[6];
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}factory,*pfactory;


#endif
