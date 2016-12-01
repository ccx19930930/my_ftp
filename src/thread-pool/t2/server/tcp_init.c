#include"head.h"
int tcp_init(int port,int num)//TCP相关初始化
{
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		exit(-1);
	}
	int ret;
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=INADDR_ANY;
	addr.sin_port=htons(port);
	ret=bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("bind");
		exit(-1);
	}
	ret=listen(sfd,num);
	if(-1==ret)
	{
		perror("listen");
		exit(-1);
	}
	printf("tcp_init success!\n");
	return sfd;
}
