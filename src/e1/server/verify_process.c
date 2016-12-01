#include"func.h"

int verify_process(puser u)
{
	char buf;
	int ret;
	ret=recv(u->sfd,&buf,sizeof(buf),0);
	if(ret<=0)
	{
		return -1;
	}
	int check;
	check=0;
	ret=send(u->sfd,&check,sizeof(check),0);
	if(ret<=0)
	{
		return -1;
	}
	strcpy(u->user,"ccx");
	strcpy(u->pdw,"ccx");
	return 1;
}
