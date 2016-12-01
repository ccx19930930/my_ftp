#include"queue.h"

void queue_init(pqueue p)
{
	p->phead=(pNode)calloc(1,sizeof(Node));
	p->phead->pnext=NULL;
	p->ptail=p->phead;
	int ret;
	ret=pthread_mutex_init(&p->mutex,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	p->size=0;
}
void enqueue(pqueue p,pNode q)
{
	q->pnext=p->ptail->pnext;
	p->ptail->pnext=q;
	p->ptail=q;
	p->size++;
}

void dequeue(pqueue p,pNode *q)
{
	*q=p->phead->pnext;
	p->phead->pnext=(*q)->pnext;
	if(*q==p->ptail)
	{
		p->ptail=p->phead;
	}
	p->size--;
}
