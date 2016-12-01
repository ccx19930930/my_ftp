#include"func.h"

int verify_password(int sfd)
{
	char buf;
	int ret;
	ret=recv(sfd,&buf,sizeof(buf),0);
	printf("buf=%c\n",buf);
	if(-1==ret)
	{
		perror("recv");
		return -1;
	}
	if(0==ret)
	{
		printf("client outline\n");
		return 0;
	}
	return 1;
}
