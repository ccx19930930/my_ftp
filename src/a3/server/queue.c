#include"func.h"

extern p_wait_queue phead;
extern p_wait_queue ptail;

void queue_init()
{
	phead=(p_wait_queue)calloc(1,sizeof(wait_queue));
	phead->pnext=NULL;
	ptail=phead;
}

void enqueue(p_wait_queue p)
{
	p->pnext=ptail->pnext;
	ptail->pnext=p;
	ptail=p;
}

void dequeue(p_wait_queue *p)
{
	*p=phead->pnext;
	phead->pnext=(*p)->pnext;
	if(*p==ptail)
	{
		ptail=phead;
	}
}

p_wait_queue queue_delete(p_wait_queue pcur,p_wait_queue ppre)
{
	if(pcur==ptail)
	{
		ptail=ppre;
	}
	ppre->pnext=pcur->pnext;
	free(pcur);
	pcur=ppre->pnext;
	return pcur;
}
