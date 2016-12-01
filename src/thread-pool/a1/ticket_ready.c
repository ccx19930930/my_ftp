#include"func.h"
void *ticket_ready(void *v)//就绪队列通知排队线程
{
	p_ready_queue p;
	p_ready_queue q;
	int ret;
	int i;
	while(1)
	{
		printf("1\n");
		p=ready_phead->pnext;
		q=ready_phead;
		i=1;
		pthread_mutex_lock(&mutex_ready);
		while(p)
		{
			printf("2\n");
			//printf("temp:\n");
			ret=send(p->sfd,(void*)&i,sizeof(i),0);
			//printf("ret=%d i=%d\n",ret,i);//测试用
			if(-1==ret)
			{
				ready_queue_delete(&p,q);
				continue;
			}
			p=p->pnext;
			q=q->pnext;
			i++;
		}
		pthread_mutex_unlock(&mutex_ready);
		sleep(1);
	}
}
