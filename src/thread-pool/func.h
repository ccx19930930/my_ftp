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

#define IP "192.168.1.114"
#define PORT 2000
#define N 5
#define M 100
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

