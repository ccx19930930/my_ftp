#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/select.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<signal.h>
#include<pthread.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

#define N 5
#define M 100

typedef struct wait_queue_struct
{
	int sfd;
	char account[16];
	char user_name[16];
	int flag;//标记位 已登陆为1
	char pwd[1024];
	struct wait_queue_struct *pnext;
}wait_queue,*p_wait_queue;

typedef struct send_file_queue
{
	int sfd;//发给谁
	int seek;//当前位置
	int total;//一共要发多少
	struct send_file_queue *pnext;
}send_queue,*p_send_queue;

typedef struct
{
	int port;
	int pthread_num;
	int download_pthread_num;
}conf_struct;

void pthread_init(pthread_t *,int);
int tcp_init(int,int);
int epoll_init(int);
void epoll_add(int,int);
void epoll_delete(int,int);
void *pthread_server(void *);
void queue_init();
void enqueue(p_wait_queue);
void dequeue(p_wait_queue *);
p_wait_queue queue_delete(p_wait_queue,p_wait_queue);
int verify_password(p_wait_queue);
void *pthread_wait_queue(void *);
void get_conf(conf_struct *);
