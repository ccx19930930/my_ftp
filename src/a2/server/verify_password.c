#include"func.h"

int verify_password(p_wait_queue p)
{
	char buf;
	int ret;
	ret=recv(p->sfd,&buf,sizeof(buf),0);
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
	strcpy(p->account,"ccx");
	strcpy(p->user_name,"ccx");
	strcpy(p->pwd,p->user_name);
	return 1;
}
