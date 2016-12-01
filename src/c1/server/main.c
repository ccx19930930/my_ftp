#include"func.h"

int main(int argc,char *argv[])
{
	if(2!=argc)
	{
		printf("error argc!\n");
		return -1;
	}
	printf("conf = %s\n",argv[1]);
	conf_struct conf;
	get_conf(&conf,argv[1]);
	printf("port=%d\nlisten_num=%d\npthread_num=%d\n",conf.port,conf.listen_num,conf.pthread_num);
	int pid;
	int fds[2];
	int ret;
	ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
	if(-1==ret)
	{
		perror("socketpair");
		return -1;
	}
	pid=fork();
	if(!pid)
	{
		close(fds[1]);
		sub_process(conf,fds[0]);
	}
	close(fds[0]);
	main_process(conf,pid,fds[1]);
}
