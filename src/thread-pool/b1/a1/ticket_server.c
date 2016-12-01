#include"func.h"
#define IP "192.168.1.114"
#define PORT 2000
#define N 5
typedef struct wait_queue_struct
{
	int sfd;
	struct wait_queue_struct *pnext;
}wait_queue,*p_wait_queue;

typedef struct ready_queue_struct
{
	int sfd;
	struct ready_queue_struct *pnext;
}ready_queue,*p_ready_queue;

p_wait_queue wait_phead;
p_wait_queue wait_ptail; 
p_ready_queue ready_phead;
p_ready_queue ready_ptail;
pthread_mutex_t mutex;
pthread_mutex_t mutex_epoll_wait;
pthread_cond_t cond;
int sfd;
int new_fd;
int epfd_accept;
int epfd_ready;
int epfd_wait;
pthread_t pthid[N];

void epoll_add(int epfd,int fd)
{
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	printf("epoll_add success!\n");
}
void epoll_delete(int epfd,int fd)
{
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
	printf("epoll_delete success!\n");
}


void queue_init()
{
	ready_phead=(p_ready_queue)calloc(0,sizeof(ready_queue));
	ready_ptail=ready_phead;
	wait_phead=(p_wait_queue)calloc(0,sizeof(wait_queue));
	wait_ptail=wait_phead;
	printf("queue_init success!\n");
}

void ready_enqueue(int fd)
{
	p_ready_queue p;
	p=(p_ready_queue)calloc(0,sizeof(ready_queue));
	p->sfd=fd;
	p->pnext=ready_ptail->pnext;
	ready_ptail->pnext=p;
	ready_ptail=p;
	pthread_cond_signal(&cond);
}
void ready_dequeue(int *fd)
{
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
	free(p);
}
void ready_queue_delete(p_ready_queue *p,p_ready_queue q)
{
	q->pnext=(*p)->pnext;
	free (*p);
	(*p)=q->pnext;
	if((*p)==ready_ptail)
	{
		ready_ptail=q;
	}
}


void wait_enqueue(int fd)
{
	p_wait_queue p;
	p=(p_wait_queue)calloc(0,sizeof(wait_queue));
	p->sfd=fd;
	p->pnext=wait_ptail->pnext;
	wait_ptail->pnext=p;
	wait_ptail=p;
}
void wait_dequeue(int *fd)
{
	p_wait_queue p;
	p=wait_phead->pnext;
	if(NULL==p)
	{
		printf("empty queue\n");
		return ;
	}
	wait_phead->pnext=p->pnext;
	*fd=p->sfd;
	if(wait_ptail==p)
	{
		wait_ptail=wait_phead;
	}
	free(p);	
}

void wait_queue_delete(p_wait_queue *p,p_wait_queue q)
{
	q->pnext=(*p)->pnext;
	free (*p);
	(*p)=q->pnext;
	if((*p)==wait_ptail)
	{
		wait_ptail=q;
	}
}
void *ticket_server(void *v)
{
	while(1){}//测试排队用  测试通过即删除  本函数以下代码已测试通过
lable_begin:
	pthread_mutex_lock(&mutex);
	if(NULL==ready_phead->pnext)
	{
		pthread_cond_wait(&cond,&mutex);
	}
	int new_fd;
	ready_dequeue(&new_fd);
	pthread_mutex_unlock(&mutex);
	int buf;
	buf=0;
	int ret;
	ret=send(new_fd,(void*)&buf,sizeof(buf),0);
	if(-1==ret)
	{
		goto lable_begin;
	}
	printf("new_fd=%d login\n",new_fd);
	sleep(10);
	printf("new_fd=%d logout\n",new_fd);
	goto lable_begin;
}

void *ticket_ready(void *v)
{
	p_ready_queue p;
	p_ready_queue q;
	int ret;
	int i;
	while(1)
	{
		p=ready_phead->pnext;
		q=ready_phead;
		i=1;
		while(p)
		{
			printf("temp:\n");
			ret=send(p->sfd,(void*)&i,sizeof(i),0);
			printf("ret=%d i=%d\n",ret,i);//测试用
			if(-1==ret)
			{
				ready_queue_delete(&p,q);
				continue;
			}
			p=p->pnext;
			q=q->pnext;
			i++;
		}
		sleep(1);
	}
}
void *ticket_wait(void *v)
{
	int fd;
	struct epoll_event event;
	int event_len;
	int ret;
	int temp;//测试
	while(1)
	{
		event_len=sizeof(event);
		memset(&event,0,event_len);
		pthread_mutex_lock(&mutex_epoll_wait);
		printf("lock mmm\n");
		ret=epoll_wait(epfd_wait,&event,event_len,1000);
		pthread_mutex_unlock(&mutex_epoll_wait);
		if(ret==0)
		{
			continue;
		}

		fd=event.data.fd;
		recv(fd,(void*)&temp,sizeof(temp),0);
		printf("recv=%d\n",temp);
		wait_dequeue(&fd);
		ready_enqueue(fd);
		epoll_add(epfd_ready,fd);
		epoll_delete(epfd_wait,fd);
	}
}
void pthread_init()
{
	int ret;
	ret=pthread_mutex_init(&mutex,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_mutex_init(&mutex_epoll_wait,NULL);
	if(0!=ret)
	{
		printf("pthread_mutex_init mutex_epoll_wait errno=%d\n",ret);
		exit(-1);
	}
	ret=pthread_cond_init(&cond,NULL);
	if(0!=ret)
	{
		printf("pthread_cond_init errno=%d\n",ret);
		exit(-1);
	}
	pthread_t pthid_ready;
	ret=pthread_create(&pthid_ready,NULL,ticket_ready,NULL);
	if(-1==ret)
	{
		printf("pthread_create pthid_ready errno=%d\n",ret);
		exit(-1);
	}
	pthread_t pthid_wait;
	ret=pthread_create(&pthid_wait,NULL,ticket_wait,NULL);
	if(-1==ret)
	{
		printf("pthread_create pthid_wait errno=%d\n",ret);
		exit(-1);
	}
	int i;
	for(i=0;i<5;i++)
	{
		ret=pthread_create(&pthid[i],NULL,ticket_server,NULL);
		if(0!=ret)
		{
			printf("pthread_create pthin[%d] errno=%d\n",i,ret);
			exit(-1);
		}
	}
	printf("pthread_init success!\n");
}

void tcp_init()
{
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		exit(-1);
	}
	int ret;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(IP);
	addr.sin_port=htons(PORT);
	ret=bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("bind");
		exit(-1);
	}
	ret=listen(sfd,100);
	if(-1==ret)
	{
		perror("listen");
		exit(-1);
	}
	printf("tcp_init success!\n");
}
void epoll_init()
{
	epfd_accept=epoll_create(1);
	epoll_add(epfd_accept,sfd);
	epfd_wait=epoll_create(1);
	epfd_ready=epoll_create(1);
}


int main()
{
	signal(SIGPIPE,SIG_IGN);
	queue_init();
	pthread_init();
	tcp_init();
	epoll_init();
	int ret;
	struct epoll_event event;
	int event_len;
	struct sockaddr_in addr;
	int addr_len;
	int i=0;
	while(1)
	{
		event_len=sizeof(event);
		memset(&event,0,event_len);
		addr_len=sizeof(addr);
		memset(&addr,0,addr_len);
		ret=epoll_wait(epfd_accept,&event,event_len,-1);
		if(ret>0)
		{
			new_fd=accept(sfd,(struct sockaddr*)&addr,&addr_len);
			if(-1==new_fd)
			{
				perror("accept");
				exit(-1);
			}
			printf("new login\n");//测试用
			wait_enqueue(new_fd);
			printf("wait_queue\n");
			pthread_mutex_lock(&mutex_epoll_wait);
			printf("lock mu\n");
			epoll_add(epfd_wait,new_fd);
			pthread_mutex_unlock(&mutex_epoll_wait);
		}
	}
}

