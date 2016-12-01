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
	if(d.buf[0]=='~')
	{
		memset(pwd,0,sizeof(pwd));
		strcpy(pwd,u[sfd].user);
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s","select id from files where MD5 = \"N\" and pathname = \"",pwd,"\"");
		//printf("select cd = %s\n",selects);
		check=mysql_select(selects,&mysql_data);
		memset(pwd_id,0,sizeof(pwd_id));
		strcpy(pwd_id,mysql_data[0].buf);
		if(d.buf[1]=='\0')
		{
			goto lable;
		}else{
			c2=d.buf+2;
		}

	}else{
		c2=d.buf;
	}
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
				send(sfd,(void*)&temp,sizeof(temp),0);
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
			sprintf(selects,"%s%s%s","select id from files where MD5 = \"N\" and pathname = \"",c3,"\"");
			//printf("select cd = %s\n",selects);
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
		sprintf(selects,"%s%s%s%s%s%s","select id from files f LEFT OUTER JOIN data d ON f.id=d.sub_file where MD5 = \"N\" and d.file = ",pwd_id," and f.pathname= \"",c2,"\" and d.user = ",u[sfd].user_id);
		//printf("select cd = %s\n",selects);
		check=mysql_select(selects,&mysql_data);
		if(0==check)
		{
			temp=0;
			send(sfd,(void*)&temp,sizeof(temp),0);
			free(mysql_data);
			//printf("pwd = %s\n",pwd);
			return ;
		}
		memset(pwd_id,0,sizeof(pwd_id));
		strcpy(pwd_id,mysql_data[0].buf);
		sprintf(pwd,"%s%s%s",pwd,"/",c2);
		//printf("pwd = %s\n",pwd);
		free(mysql_data);
		if(NULL!=c1)
		{
			c2=c1+1;
		}
	}
lable:
	temp=1;
	send(sfd,(void*)&temp,sizeof(temp),0);
	memset(u[sfd].pwd,0,sizeof(u[sfd].pwd));
	memset(u[sfd].pwd_id,0,sizeof(u[sfd].pwd_id));
	strcpy(u[sfd].pwd,pwd);
	strcpy(u[sfd].pwd_id,pwd_id);
	//printf("final pwd=%s\nfinal pwd_id=%s\n",u[sfd].pwd,u[sfd].pwd_id);
	return ;
}

void server_ls(int sfd)
{
	data d;
	data *mysql_data;
	memset(&d,0,sizeof(d));
	int temp;
	int i;
	char selects[1024];
	unsigned long check;
	sprintf(selects,"%s%s%s%s%s","select pathname from files f LEFT OUTER JOIN data d ON f.id=d.sub_file where d.file = ",u[sfd].pwd_id," and d.user = ",u[sfd].user_id," ;");
	//	printf("select cd = %s\n",selects);
	check=mysql_select(selects,&mysql_data);
	if(0==check)
	{
		temp=0;
		send(sfd,(void*)&temp,sizeof(temp),0);
		free(mysql_data);
		return ;
	}
	send(sfd,(void*)&check,sizeof(check),0);
	for(i=0;i<check;i++)
	{
		memset(&d,0,sizeof(d));
		strcpy(d.buf,mysql_data[i].buf);
		d.len=strlen(d.buf);
		send_protocol(sfd,&d);
	}
	return ;
}


void server_mkdir(int sfd)
{
	data d;
	data *mysql_data;
	memset(&d,0,sizeof(d));
	int ret;
	char new_dir_id[4];
	char selects[1024];
	unsigned long check;
	ret=recv_protocol(sfd,&d);
	if(-1==ret)
	{
		return ;
	}
	//	printf("mkdir=%s\n",d.buf);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s","insert into files (pathname,MD5) values (\"",d.buf,"\",\"N\");");
	//	printf("select mkdir = %s\n",selects);
	mysql_add_update_delete(selects);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s","select id from files where MD5 = \"N\" and pathname = \"",d.buf,"\"");
	//	printf("select mkdir = %s\n",selects);
	check=mysql_select(selects,&mysql_data);
	memset(new_dir_id,0,sizeof(new_dir_id));
	strcpy(new_dir_id,mysql_data[0].buf);
	free(mysql_data);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s%s%s","insert into data (user,file,sub_file) values (",u[sfd].user_id,",",u[sfd].pwd_id,",",new_dir_id,");");
	//	printf("select mkdir = %s\n",selects);
	mysql_add_update_delete(selects);
	ret=1;
	send(sfd,(void*)&ret,sizeof(ret),0);
}

void server_remove(int sfd)
{
	data d;
	data *mysql_data;
	memset(&d,0,sizeof(d));
	int ret;
	int temp;
	char remove_no[4];
	char selects[1024];
	unsigned long check;
	ret=recv_protocol(sfd,&d);
	if(-1==ret)
	{
		return ;
	}
	printf("remove=%s\n",d.buf);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s%s","select no from data d LEFT OUTER JOIN files f ON f.id=d.sub_file where d.file = ",u[sfd].pwd_id," and f.pathname= \"",d.buf,"\" and d.user = ",u[sfd].user_id);
	//	printf("select remove = %s\n",selects);
	check=mysql_select(selects,&mysql_data);
	if(0==check)
	{
		temp=0;
		send(sfd,(void*)&temp,sizeof(temp),0);
		free(mysql_data);
		return ;
	}
	memset(remove_no,0,sizeof(remove_no));
	strcpy(remove_no,mysql_data[0].buf);
	free(mysql_data);
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s","delete from data where no = ",remove_no,";");
	//	printf("select remove = %s\n",selects);
	mysql_add_update_delete(selects);
	temp=1;
	send(sfd,(void*)&temp,sizeof(temp),0);
}

void server_gets(int sfd,file_serve_struct *f_get)
{
	int temp;
	data d;
	data *mysql_data;
	char selects[1024];
	char new_file_id[4];
	char md5[40];
	off_t seek;
	unsigned long check;
	memset(&d,0,sizeof(d));
	recv_protocol(sfd,&d);
	sprintf(selects,"%s%s%s%s%s%s","select md5 from files f LEFT OUTER JOIN data d ON f.id=d.sub_file where d.file = ",u[sfd].pwd_id," and f.pathname= \"",d.buf,"\" and d.user = ",u[sfd].user_id);
	check=mysql_select(selects,&mysql_data);
	if(0==check)
	{
		temp=0;
		send(sfd,(void*)&temp,sizeof(temp),0);
		free(mysql_data);
		return ;
	}
	memset(md5,0,sizeof(md5));
	strcpy(md5,mysql_data[0].buf);
	free(mysql_data);
	int fd;
	fd=open(md5,O_RDONLY);
	if(-1==fd)
	{
		temp=-1;
		send(sfd,(void*)&temp,sizeof(temp),0);
		return;
	}
	temp=1;
	send(sfd,(void*)&temp,sizeof(temp),0);
	recv_n(sfd,(void*)&seek,8);
	struct stat buf;
	if(0==seek)
	{
		stat(md5,&buf);
		send_n(sfd,(void*)&buf.st_mode,2);
	}
	p_file_queue p;
	p=(p_file_queue)calloc(1,sizeof(file_queue));
	p->sfd=sfd;
	p->fd=fd;
	p->seek=seek;
	p->num=1024*64;
	p->pnext=NULL;
	pthread_mutex_lock(&f_get->mutex);
	file_enqueue(&f_get->f,p);
	pthread_mutex_unlock(&f_get->mutex);
	pthread_cond_signal(&f_get->cond);
}

int server_puts(int sfd,file_serve_struct *f_put)
{
	int temp;
	data d;
	int sign=0;
	data *mysql_data;
	char selects[1024];
	char new_file_id[4];
	char md5[40];
	unsigned long check;
	memset(&d,0,sizeof(d));
	recv_protocol(sfd,&d);
	memset(md5,0,sizeof(md5));
	strcpy(md5,d.buf);
	memset(&d,0,sizeof(d));
	recv_protocol(sfd,&d);



//查询files表,如不存在则补上
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s","select id from files where MD5 = \"",md5,"\" and pathname = \"",d.buf,"\"");
	check=mysql_select(selects,&mysql_data);
	if(0==check)//文件表中无此数据 要补上
	{
		free(mysql_data);
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s%s%s","insert into files (pathname,MD5) values (\"",d.buf,"\",\"",md5,"\");");
		mysql_add_update_delete(selects);
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s%s%s","select id from files where MD5 = \"",md5,"\" and pathname = \"",d.buf,"\"");
		check=mysql_select(selects,&mysql_data);
	}else{
		sign++;
	}
	memset(new_file_id,0,sizeof(new_file_id));
	strcpy(new_file_id,mysql_data[0].buf);
	free(mysql_data);


//查询data表,如不存在则补上
	memset(selects,0,sizeof(selects));
	sprintf(selects,"%s%s%s%s%s%s","select no from data where file = ",u[sfd].pwd_id," and sub_file = ",new_file_id," and user = ",u[sfd].user_id);
	check=mysql_select(selects,&mysql_data);
	free(mysql_data);
	if(0==check)
	{
		memset(selects,0,sizeof(selects));
		sprintf(selects,"%s%s%s%s%s%s%s","insert into data (user,file,sub_file) values (",u[sfd].user_id,",",u[sfd].pwd_id,",",new_file_id,");");
		mysql_add_update_delete(selects);
	}else{
		sign++;
	}


//查询文件信息
	struct stat buf;
	memset(&buf,0,sizeof(buf));
	int ret;
	ret=stat(md5,&buf);
	send_n(sfd,(char*)&buf.st_size,sizeof(off_t));
	recv(sfd,(void*)&temp,sizeof(temp),0);
	if(0==temp)//说明服务器文件正确
	{
		sign++;
		if(3==sign)
		{
			send(sfd,(void*)&temp,sizeof(temp),0);
			return 0;
		}else{
			temp=1;
			send(sfd,(void*)&temp,sizeof(temp),0);
			return 0;
		}
	}
	mode_t mode=0666;
	recv_n(sfd,(void*)&mode,2);
	int fd;
	fd=open(md5,O_WRONLY|O_CREAT,mode);
	if(-1==fd)
	{
		perror("open");
		return 0;
	}
	p_file_queue p;
	p=(p_file_queue)calloc(1,sizeof(file_queue));
	p->sfd=sfd;
	p->fd=fd;
	p->seek=buf.st_size;
	p->num=1024*64;
	p->pnext=NULL;
	pthread_mutex_lock(&f_put->mutex);
	file_enqueue(&f_put->f,p);
	pthread_mutex_unlock(&f_put->mutex);
	return 1;
}
