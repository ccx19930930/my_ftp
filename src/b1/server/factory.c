#include"func.h"

void factory_init(pfactory p)
{
	int ret;
	p->pth_fun[0]=ls_thread;
	p->pth_fun[1]=cd_thread;
	p->pth_fun[2]=pdw_thread;
	p->pth_fun[3]=puts_thread;
	p->pth_fun[4]=gets_thread;
	p->pth_fun[5]=remove_thread;
	ret=pthread_mutex_init(&p->mutex,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_cond_init(&p->cond,NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init errno=%d\n",ret);
		exit(-1);
	}
}

void factory_start(pfactory p)
{
	int ret;
	int i;
	for(i=0;i<6;i++)
	{
		ret=pthread_create(&p->pthid[i],NULL,p->pth_fun[i],(void*)p);
		if(0!=ret)
		{
			printf("pthread_create errno=%d\n",ret);
			exit(-1);
		}
	}
}
