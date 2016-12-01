#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef struct
{
	unsigned int len;
	char buf[1024];
}data;

typedef struct send_file_queue
{
	int sfd;//扔给谁的包/谁扔过来的包
	int fd;//要扔的文件描述符
	off_t seek;//当前位置
	int num;//扔多少
	struct send_file_queue *pnext;//下个包
}file_queue,*p_file_queue;

typedef struct //链表
{
	p_file_queue phead;
	p_file_queue ptail;
}file_queue_type;






int recv_protocol(int sfd,data *d);
int send_protocol(int sfd,data *d);
int send_n(int sfd,char *p,int len);
int recv_n(int sfd,char *p,int len);
void *pthread_gets(void *v);
void *pthread_puts(void *v);

#endif
