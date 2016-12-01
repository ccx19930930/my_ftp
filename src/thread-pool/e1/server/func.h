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
	struct wait_queue_struct *pnext;
}wait_queue,*p_wait_queue;

void pthread_init(pthread_t *,int);
int tcp_init(int,int);
int epoll_init(int);
void epoll_add(int,int);
void epoll_delete(int,int);
void *pthread_server(void *);
void queue_init(int);
void enqueue(int);
void dequeue(int *);
int verify_password(int);
void *pthread_wait_queue(void *);
