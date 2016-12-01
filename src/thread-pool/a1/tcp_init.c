#include"func.h"
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
	ret=listen(sfd,M);
	if(-1==ret)
	{
		perror("listen");
		exit(-1);
	}
	printf("tcp_init success!\n");
}
