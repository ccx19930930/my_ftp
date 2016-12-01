#include"func.h"

extern p_wait_queue phead;
extern p_wait_queue ptail;

void queue_init(int num)
{
	phead=(p_wait_queue)calloc(1,sizeof(wait_queue));
	phead->pnext=NULL;
	ptail=phead;
}

void enqueue(int fd)
{
	p_wait_queue p;
	p=(p_wait_queue)malloc(sizeof(wait_queue));
	p->sfd=fd;
	p->pnext=ptail->pnext;
	ptail->pnext=p;
	ptail=p;
}

void dequeue(int *fd)
{
	p_wait_queue p;
	p=phead->pnext;
	phead->pnext=p->pnext;
	if(p==ptail)
	{
		ptail=phead;
	}
	*fd=p->sfd;
	free(p);
}
