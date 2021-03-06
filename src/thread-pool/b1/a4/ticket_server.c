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
int pipe_fd[2];
int epfd_accept;
int epfd_ready;
int epfd_wait;
pthread_t pthid[N];

int send_fd(int sfdw,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	char buf1[10]="hello";
	char buf2[10]="world";
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	size_t len;
	len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=sendmsg(sfdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
	//	close(sfdw);
}
int recv_fd(int sfdr,int *fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	char buf1[10];
	char buf2[10];
	struct iovec iov[2];
	iov[0].iov_base=buf1;
	iov[0].iov_len=5;
	iov[1].iov_base=buf2;
	iov[1].iov_len=5;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	size_t len;
	len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=recvmsg(sfdr,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
	*fd=*(int*)CMSG_DATA(cmsg);
	//	close(sfdr);
}
void epoll_add(int epfd,int fd)
{
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl_add");
	}else{
		printf("epoll_add %d success!\n",fd);//测试
	}
}
void epoll_delete(int epfd,int fd)
{
	int ret;
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	ret=epoll_ctl(epfd,EPOLL_CTL_DEL,sfd,&event);
	if(-1==ret)
	{
		perror("epoll_ctl_delete");
	}else{
		close(fd);
		printf("epoll_delete %d success!\n",fd);//测试
	}
}


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
	p_ready_queue p;
	p=(p_ready_queue)calloc(0,sizeof(ready_queue));
	p->sfd=fd;
	p->pnext=ready_ptail->pnext;
	ready_ptail->pnext=p;
	ready_ptail=p;
	pthread_cond_signal(&cond);
}
void ready_dequeue(int *fd)//就绪队列出队
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
void ready_queue_delete(p_ready_queue *p,p_ready_queue q)//就绪队列删其中一个元素,链表操作,用于等待的客户端断开连接时移出队列
{
	q->pnext=(*p)->pnext;
	free (*p);
	(*p)=q->pnext;
	if((*p)==ready_ptail)
	{
		ready_ptail=q;
	}
}


void wait_enqueue(int fd)//等待队列入队(未验证帐号密码)
{
	p_wait_queue p;
	p=(p_wait_queue)calloc(0,sizeof(wait_queue));
	p->sfd=fd;
	p->pnext=wait_ptail->pnext;
	wait_ptail->pnext=p;
	wait_ptail=p;
}
void wait_dequeue(int *fd)//等待队列出队
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

void wait_queue_delete(p_wait_queue *p,p_wait_queue q)//等待队列删除元素
{
	q->pnext=(*p)->pnext;
	free (*p);
	(*p)=q->pnext;
	if((*p)==wait_ptail)
	{
		wait_ptail=q;
	}
}
void *ticket_server(void *v)//服务线程
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

void *ticket_ready(void *v)//就绪队列通知排队线程
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
void *ticket_wait(void *v)//等待队列->就绪队列处理  BUG线程
{
	int fd;
	struct epoll_event event;
	int event_len;
	int ret;
	int temp;//测试
	event_len=1;
	while(1)
	{
		memset(&event,0,sizeof(event));
		ret=epoll_wait(epfd_wait,&event,event_len,-1);
		printf("ret=%d\n",ret);
		printf("event.data.fd=%d\n",event.data.fd);
		if(event.data.fd==pipe_fd[0])
		{
			//	read(pipe_fd[0],&fd,sizeof(fd));
			recv_fd(pipe_fd[0],&fd);
			epoll_add(epfd_wait,fd);
			event_len++;
		}else{
			printf("ok\n");
			fd=event.data.fd;
			recv(fd,(void*)&temp,sizeof(temp),0);
			printf("recv=%d\n",temp);
			wait_dequeue(&fd);
			ready_enqueue(fd);
			epoll_add(epfd_ready,fd);
			epoll_delete(epfd_wait,fd);
			event_len--;
		}
	}
}
void pthread_init()//线程相关初始化
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

void tcp_init()//TCP相关初始化
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
void epoll_init()//epoll相关初始化
{
	epfd_accept=epoll_create(1);
	epoll_add(epfd_accept,sfd);
	epfd_wait=epoll_create(1);
	epoll_add(epfd_wait,pipe_fd[0]);
	epfd_ready=epoll_create(1);
}


int main()
{
	//	pipe(pipe_fd);
	socketpair(AF_LOCAL,SOCK_STREAM,0,pipe_fd);
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
		new_fd=0;
		event_len=1;
		memset(&event,0,sizeof(event));
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
			printf("IP:%s\tPORT:%d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
			wait_enqueue(new_fd);
			send_fd(pipe_fd[1],new_fd);
			close(new_fd);
			//	write(pipe_fd[1],&new_fd,sizeof(new_fd));
		}
	}
}

