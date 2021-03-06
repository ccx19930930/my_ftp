#ifndef __head_h_
#define __head_h_

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
#define DOWNLOAD "file.txt"

typedef struct 
{
	 int len;
	 char buf[N];
}data;

int tcp_init(int,int);

#endif

