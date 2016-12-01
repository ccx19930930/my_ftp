#include"func.h"

extern puser u;

void server_pwd(int sfd)
{
	data d;
	strcpy(d.buf,u[sfd].pwd);
	d.len=strlen(d.buf);
	send_protocol(sfd,&d);
}

void server_cd(int sfd)
{
	data d;
	data *mysql_data;
	memset(&d,0,sizeof(d));
	int ret;
	int temp;
	char selects[1024];
	unsigned long check;

	ret=recv_protocol(sfd,&d);
	if(-1==ret)
	{
		return ;
	}
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s","select id from files f LEFT OUTER JOIN data d ON f.id=d.sub_file where d.file = ",u[sfd].pwd_id," and f.pathname= \"",d.buf,"\";");
	printf("select cd = %s\n",selects);
	check=mysql_select(selects,&mysql_data);
	if(0==check)
	{
		temp=0;
		send(sfd,&temp,sizeof(temp),0);
		free(mysql_data);
		printf("pwd = %s\n",u[sfd].pwd);
		return ;
	}
	temp=1;
	send(sfd,&temp,sizeof(temp),0);
	strcpy(u[sfd].pwd_id,mysql_data[0].buf);
	sprintf(u[sfd].pwd,"%s%s%s",u[sfd].pwd,"/",d.buf);
	printf("pwd = %s\n",u[sfd].pwd);
	free(mysql_data);
	return ;
}

