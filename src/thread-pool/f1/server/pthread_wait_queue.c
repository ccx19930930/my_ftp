#include"func.h"

extern p_wait_queue phead;
extern p_wait_queue ptail;

void *pthread_wait_queue(void *v)
{
	int i;
	p_wait_queue p;
	int ret;
	while(1)
	{	
		i=1;
		p=phead->pnext;
		sleep(1);
		while(p)
		{
			printf("i=%d\n",i);
			ret=send(p->sfd,&i,sizeof(i),0);
			if(-1==ret)
			{
				perror("send");
			}
			i++;
			p=p->pnext;
		}
	}
}




		
