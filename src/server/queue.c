#include"func.h"

void file_queue_init(file_queue_type *f)
{
	f->phead=(p_file_queue)calloc(1,sizeof(file_queue));
	f->phead->pnext=NULL;
	f->ptail=f->phead;
}


void file_enqueue(file_queue_type *f,p_file_queue p)//入队列
{
	p->pnext=f->ptail->pnext;
	f->ptail->pnext=p;
	f->ptail=p;
//	printf("enqueue success!\n");
}

void file_dequeue(file_queue_type *f,p_file_queue *p)
{
	(*p)=f->phead->pnext;
	if(*p==f->ptail)
	{
		f->ptail=f->phead;
	}
	f->phead->pnext=(*p)->pnext;
//	printf("dequeue success!\n");
}

void file_queue_delete(file_queue_type *f,p_file_queue pcur,p_file_queue ppre)//从链表中删除
{
	if(pcur==f->ptail)
	{
		f->ptail=ppre;
	}
	ppre->pnext=pcur->pnext;
	free(pcur);
}
