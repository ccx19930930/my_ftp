#include"func.h"

int verify_process(puser u)
{
	data d[2];
	//char *selects;
	char selects[1024];
	int ret;
	unsigned long check;
	data *mysql_data;
	ret=recv_protocol(u->sfd,&d[0]);
	if(-1==ret)
	{
		return -1;
	}
	ret=recv_protocol(u->sfd,&d[1]);
	if(-1==ret)
	{
		return -1;
	}
	//selects=(char*)calloc(1,d[0].len+d[1].len+100);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s","select id from users where user = \"",d[0].buf,"\" and password= \"",d[1].buf,"\";");
	check=mysql_select(selects,&mysql_data);
	send_n(u->sfd,(char*)&check,4);
	if(1==check)
	{
		strcpy(u->user_id,mysql_data[0].buf);
		free(mysql_data);
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s","select id from files where pathname = \"",d[0].buf,"\";");
		mysql_select(selects,&mysql_data);
		strcpy(u->pwd_id,mysql_data[0].buf);
		free(mysql_data);
		strcpy(u->user,d[0].buf);
		strcpy(u->pwd,d[0].buf);
		strcpy(u->lastpwd,u->pwd);
		strcpy(u->lastid,u->pwd_id);
		return 1;
	}
	return 0;
}
