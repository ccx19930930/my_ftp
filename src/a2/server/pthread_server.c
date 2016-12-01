#include"func.h"

extern p_wait_queue phead;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;


void *pthread_server(void *v)
{
	while(1)
	{
		p_wait_queue p;
		pthread_mutex_lock(&mutex);
		if(NULL==phead->pnext)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		dequeue(&p);
		pthread_mutex_unlock(&mutex);
		printf("sfd = %d begin to work\n",p->sfd);
		int temp=0;
		int ret;
		ret=send(p->sfd,&temp,sizeof(temp),0);
		if(-1==ret)
		{
			close(p->sfd);
			printf("pthread client outline(send)\n");
			printf("close = %d\n",p->sfd);
			continue;
		}
		ret=recv(p->sfd,&temp,sizeof(temp),0);
		if(-1==ret)
		{
			close(p->sfd);
			printf("pthread client outline(recv)\n");
			printf("close = %d\n",p->sfd);
			continue;
		}
		close(p->sfd);
		printf("close = %d\n",p->sfd);
		printf("work over\n");
	}
}	
