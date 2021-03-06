#include"func.h"

extern p_wait_queue phead;
extern p_wait_queue ptail;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern int epfd;


void *pthread_server(void *v)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(NULL==phead->pnext)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		int sfd;
		dequeue(&sfd);
		pthread_mutex_unlock(&mutex);
		printf("sfd = %d begin to work\n",sfd);
		int temp=0;
		int ret;
		ret=send(sfd,&temp,sizeof(temp),0);
		if(-1==ret)
		{
			close(sfd);
			printf("pthread client outline\n");
			continue;
		}
		ret=recv(sfd,&temp,sizeof(temp),0);
		if(-1==ret)
		{
			close(sfd);
			printf("pthread client outline\n");
			continue;
		}
		close(sfd);
		printf("work over\n");
	}
}	
