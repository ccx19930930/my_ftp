#include"factory.h"

void factory_init(pfactory p)
{
	p->pthid=(pthread_t*)calloc(p->pthread_num,sizeof(pthread_t));
	p->pthread_func=pthread_server;
	queue_init(&p->q);
	int ret;
	ret=pthread_cond_init(&p->cond,NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init errno=%d\n",ret);
		exit(-1);
	}
	p->capability=p->pthread_num;
	p->flag=0;
}

void factory_start(pfactory p)
{
	int ret;
	int i;
	if(0==p->flag)
	{
		for(i=0;i<p->pthread_num;i++)
		{
			ret=pthread_create(p->pthid+i,NULL,p->pthread_func,(void*)p);
			if(-1==ret)
			{
				printf("pthread_create errno=%d\n",ret);
				exit(-1);
			}
		}
	}
	p->flag=1;
}
