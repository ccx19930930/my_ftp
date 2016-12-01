#include"func.h"

void queue_init()//队列初始化
{
	ready_phead=(p_ready_queue)calloc(0,sizeof(ready_queue));
	ready_ptail=ready_phead;
	wait_phead=(p_wait_queue)calloc(0,sizeof(wait_queue));
	wait_ptail=wait_phead;
	printf("queue_init success!\n");
}

void ready_enqueue(int fd)//就绪队列入队
{
	printf("ready to ready_enqueue\n");
	p_ready_queue p;
	p=(p_ready_queue)calloc(0,sizeof(ready_queue));
	p->sfd=fd;
	p->pnext=ready_ptail->pnext;
	ready_ptail->pnext=p;
	ready_ptail=p;
	pthread_cond_signal(&ready_cond);
	printf("ready_enqueue success!\n");
}
void ready_dequeue(int *fd)//就绪队列出队
{
	printf("ready to ready_dequque\n");
	p_ready_queue p;
	p=ready_phead->pnext;
	if(NULL==p)
	{
		printf("empty queue\n");
		return ;
	}
	ready_phead->pnext=p->pnext;
	*fd=p->sfd;
	if(p==ready_ptail)
	{
		ready_ptail=ready_phead;
	}
	epoll_delete(epfd,*fd);
	free(p);
	printf("ready_dequeue success!\n");
}
void ready_queue_delete(p_ready_queue *p,p_ready_queue q)//就绪队列删其中一个元素,链表操作,用于等待的客户端断开连接时移出队列
{
	printf("ready to ready_queue_delete\n");
	q->pnext=(*p)->pnext;
	if((*p)==ready_ptail)
	{
		ready_ptail=q;
		ready_ptail->pnext=NULL;
