#include"func.h"

void send_n(int sfd,char *p,int len)
{
	int total=0;
	int ret;
	while(total<len)
	{
		ret=send(sfd,p+total,len-total,0);
		if(-1==ret)
		{
			perror("send");
			exit(-1);
		}
		total+=ret;
	}
}

