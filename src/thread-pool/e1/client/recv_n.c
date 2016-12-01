#include"func.h"

void recv_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=recv(sfd,p+total,len-total,0);
		if(-1==ret)
		{
			perror("recv");
			exit(-1);
		}
		total+=ret;
	}
}

