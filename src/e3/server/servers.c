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
	char pwd[1024];
	char pwd_id[4];
	memset(&pwd,0,sizeof(pwd));
	memset(&pwd_id,0,sizeof(pwd_id));
	strcpy(pwd,u[sfd].pwd);
	strcpy(pwd_id,u[sfd].pwd_id);
	char *c1;
	char *c2;
	char *c3;
	ret=recv_protocol(sfd,&d);
	if(-1==ret)
	{
		return ;
	}
	c2=d.buf;
	c1=c2;
	while(c1)
	{
		c1=strchr(c2,'/');
		if(NULL!=c1)
		{
			c1[0]='\0';
		}
		if(!strcmp(c2,"."))
		{
			if(NULL!=c1)
			{
				c2=c1+1;
			}
			continue;
		}
		if(!strcmp(c2,".."))
		{
			if(!strcmp(pwd,u[sfd].user))
			{
				temp=0;
				send(sfd,&temp,sizeof(temp),0);
				return ;
			}
			ret=strlen(pwd);
			temp=ret-1;;
			while(pwd[temp]!='/')
			{
				temp--;
			}
			memset(pwd+temp,0,sizeof(char)*(ret-temp));
			temp--;
			while(pwd[temp]!='/'&&temp>=0)
			{
				temp--;
			}
			c3=pwd+temp+1;
			memset(selects,0,sizeof(selects));
			sprintf(selects,"%s%s%s","select id from files where pathname = \"",c3,"\"");
			printf("select cd = %s\n",selects);
			check=mysql_select(selects,&mysql_data);
			memset(pwd_id,0,sizeof(pwd_id));
			strcpy(pwd_id,mysql_data[0].buf);
			free(mysql_data);
			if(NULL!=c1)
			{
				c2=c1+1;
			}
			continue;
		}
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s%s%s%s","select id from files f LEFT OUTER JOIN data d ON f.id=d.sub_file where d.file = ",pwd_id," and f.pathname= \"",c2,"\" and d.user = ",u[sfd].user_id);
		printf("select cd = %s\n",selects);
		check=mysql_select(selects,&mysql_data);
		if(0==check)
		{
			temp=0;
			send(sfd,&temp,sizeof(temp),0);
			free(mysql_data);
			printf("pwd = %s\n",pwd);
			return ;
		}
		memset(pwd_id,0,sizeof(pwd_id));
		strcpy(pwd_id,mysql_data[0].buf);
		sprintf(pwd,"%s%s%s",pwd,"/",c2);
		printf("pwd = %s\n",pwd);
		free(mysql_data);
		if(NULL!=c1)
		{
			c2=c1+1;
		}
	}
	temp=1;
	send(sfd,&temp,sizeof(temp),0);
	memset(u[sfd].pwd,0,sizeof(u[sfd].pwd));
	memset(u[sfd].pwd_id,0,sizeof(u[sfd].pwd_id));
	strcpy(u[sfd].pwd,pwd);
	strcpy(u[sfd].pwd_id,pwd_id);
	printf("final pwd=%s\nfinal pwd_id=%s\n",u[sfd].pwd,u[sfd].pwd_id);
	return ;
}

