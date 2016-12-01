#include"func.h"

void file_serve_init(file_serve_struct *fss,int num)
{
	fss->pthid=(pthread_t*)calloc(num,sizeof(pthread_t));
	int ret;
	ret=pthread_mutex_init(&fss->mutex,NULL);
	if(-1==ret)
	{
		printf("pthread_mutex_init errno = %d\n",ret);
		exit(-1);
	}
	ret=pthread_cond_init(&fss->cond,NULL);
	if(-1==ret)
	{
		printf("pthread_cond_init errno = %d\n",ret);
		exit(-1);
	}
	file_queue_init(&fss->f);
	int i;
	for(i=0;i<num;i++)
	{
		ret=pthread_create(fss->pthid+i,NULL,fss->pf,(void*)fss);
		if(-1==ret)
		{
			printf("pthread_create errno = %d\n",ret);
			exit(-1);
		}
	}
	//printf("phead=%p,ptail=%p\n",fss->f.phead,fss->f.ptail);
}

