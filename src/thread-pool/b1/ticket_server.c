#include"func.h"
void *ticket_server(void *v)//服务线程
{
	while(1){}//测试排队用  测试通过即删除  本函数以下代码已测试通过
lable_begin:
	pthread_mutex_lock(&mutex);
	if(NULL==ready_phead->pnext)
	{
		pthread_cond_wait(&ready_cond,&mutex);
	}
	int new_fd;
	ready_dequeue(&new_fd);
	pthread_mutex_unlock(&mutex);
	int buf;
	buf=0;
	int ret;
	ret=send(new_fd,(void*)&buf,sizeof(buf),0);
	if(-1==ret)
	{
		goto lable_begin;
	}
	printf("new_fd=%d login\n",new_fd);
	sleep(10);
	printf("new_fd=%d logout\n",new_fd);
	goto lable_begin;
}
