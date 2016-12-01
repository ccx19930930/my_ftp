#include"func.h"


int main(int argc,char *argv[])
{
	signal(SIGPIPE,SIG_IGN);
	if(3!=argc)
	{
		printf("error argc!\n");
		return -1;
	}
	printf("argc right\n");
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	printf("socket nuccess!\n");
	int ret;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(atoi(argv[2]));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	ret=connect(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect success!\n");
	verify_process(sfd);
	client(sfd);
}
