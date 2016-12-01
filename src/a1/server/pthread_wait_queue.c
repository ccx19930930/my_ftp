#include"func.h"

extern p_wait_queue phead;
extern p_wait_queue ptail;
extern pthread_mutex_t mutex;

void *pthread_wait_queue(void *v)
{
	signal(SIGPIPE,SIG_IGN);
	int i;
	p_wait_queue pcur;
	p_wait_queue ppre;
	int ret;
	while(1)
	{	
		i=1;
		sleep(1);
		pthread_mutex_lock(&mutex);
		pcur=phead->pnext;
		ppre=phead;
		while(pcur)
		{
			printf("i=%d,sfd=%d\n",i,pcur->sfd);
			ret=send(pcur->sfd,&i,sizeof(i),0);
			if(-1==ret)
			{
				close(pcur->sfd);
				printf("(queue)close = %d\n",pcur->sfd);
				pcur=queue_delete(pcur->sfd);
				continue;
			}
			i++;
			pcur=pcur->pnext;
			ppre=ppre->pnext;
		}
		pthread_mutex_unlock(&mutex);
	}
}




		
