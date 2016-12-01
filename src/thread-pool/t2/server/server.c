#include"factory.h"

int main(int argc,char *argv[])
{
	if(4!=argc)
	{
		printf("error argc!\nport   pthread_num   capability\n");
		return -1;
	}
	factory fac;
	fac.pthread_num=atoi(argv[2]);
	fac.capability=atoi(argv[3]);
	factory_init(&fac);
	factory_start(&fac);
	int sfd;
	int port;
	port=atoi(argv[1]);
	sfd=tcp_init(port,fac.capability);
	int new_fd;
	pNode p;
	while(1)
	{
		new_fd=accept(sfd,NULL,NULL);
		if(-1==new_fd)
		{
			perror("accept");
			return -1;
		}
		p=(pNode)malloc(sizeof(Node));
		p->sfd=new_fd;
		p->pnext=NULL;
		pthread_mutex_lock(&fac.q.mutex);
		enqueue(&fac.q,p);
		pthread_mutex_unlock(&fac.q.mutex);
		pthread_cond_signal(&fac.cond);
	}
}
