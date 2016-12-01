#include"func.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

void pthread_init(pthread_t *pthid,int num)//线程相关初始化
{
	int ret;
	ret=pthread_mutex_init(&mutex,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_cond_init(&cond,NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init ready_cond errno=%d\n",ret);
		exit(-1);
	}
	pthid=(pthread_t*)calloc(0,sizeof(pthread_t)*num);
	int i;
	for(i=0;i<num;i++)
	{
		ret=pthread_create(&pthid[i],NULL,pthread_server,NULL);
		if(0!=ret)
		{
			printf("pthread_create pthid[%d] errno=%d\n",i,ret);
			exit(-1);
		}
	}
	printf("pthread_init success!\n");
}
