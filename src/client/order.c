#include"func.h"

int order_ana(orders *o)
{
	printf("========================================\npwd cd ls mkdir gets puts remove\n");
	char buf[1024];
	memset(buf,0,sizeof(buf));
	char *c;
	int i;
	read(0,buf,sizeof(buf));
	buf[strlen(buf)-1]='\0';
	if(buf[strlen(buf)-1]=='/')
	{
		buf[strlen(buf)-1]='\0';
	}
	while(buf[strlen(buf)-1]==' ')
	{
		buf[strlen(buf)-1]='\0';
	}
	i=0;
	while(buf[i]==' ')
	{
		i++;
	}
		
	c=strchr(buf+i,' ');
	if(NULL==c)
	{
		if(!strcmp(buf+i,"pwd"))
		{
			return 1;
		}
		if(!strcmp(buf+i,"cd"))
		{
			strcpy(o->buf,"~");
			return 2;
		}
		if(!strcmp(buf+i,"ls"))
		{
			return 3;
		}
		return 10;
	}
	c[0]='\0';
	c++;
	strcpy(o->order,buf+i);
	strcpy(o->buf,c);
	if(!strcmp(o->order,"cd"))
	{
		return 2;
	}
	if(!strcmp(o->order,"mkdir"))
	{
		return 4;
	}
	if(!strcmp(o->order,"remove"))
	{
		return 5;
	}
	if(!strcmp(o->order,"gets"))
	{
		return 6;
	}
	if(!strcmp(o->order,"puts"))
	{
		return 7;
	}
	return 10;
}
