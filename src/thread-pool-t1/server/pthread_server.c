#include"factory.h"

void *pthread_server(void* v)
{	
	pfactory pf=(pfactory)v;
	pNode pn;
	int sfd;
	while(1)
	{
		pthread_mutex_lock(&pf->q.mutex);
		if(0==pf->q.size)
		{
			pthread_cond_wait(&pf->cond,&pf->q.mutex);
		}
		dequeue(&pf->q,&pn);
		pthread_mutex_unlock(&pf->q.mutex);
		printf("work begin\n");
		sfd=pn->sfd;
		free(pn);
		send_file(sfd);
		close(sfd);
		printf("work over!\n");
	}
}
