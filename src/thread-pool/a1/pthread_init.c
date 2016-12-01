#include"func.h"
void pthread_init()//线程相关初始化
{
	int ret;
	ret=pthread_mutex_init(&mutex,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_mutex_init(&mutex_ready,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_cond_init(&ready_cond,NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init ready_cond errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_create(&pthid_ready,NULL,ticket_ready,NULL);
	if(-1==ret)
	{
		printf("pthread_create pthid_ready errno=%d\n",ret);
		exit(-1);
	}
	int i;
	for(i=0;i<5;i++)
	{
		ret=pthread_create(&pthid[i],NULL,ticket_server,NULL);
		if(0!=ret)
		{
			printf("pthread_create pthin[%d] errno=%d\n",i,ret);
			exit(-1);
		}
	}
	printf("pthread_init success!\n");
}
