#ifndef __QUEUE_H__
#define __QUEUE_H__

#include"head.h"

typedef struct queue_new_sfd
{
	int sfd;
	struct queue_new_sfd *pnext;
}Node,*pNode;

typedef struct
{
	pNode phead;
	pNode ptail;
	pthread_mutex_t mutex;
	int size;
}queue,*pqueue;


void queue_init(pqueue);
void enqueue(pqueue ,pNode );
void dequeue(pqueue ,pNode *);

#endif

