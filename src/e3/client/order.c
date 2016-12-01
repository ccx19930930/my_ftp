#include"func.h"

int order_ana(orders *o)
{
	printf("pwd cd ls gets puts remove\n");
	char buf[1024];
	memset(buf,0,sizeof(buf));
	char *c;
	read(0,buf,sizeof(buf));
	buf[strlen(buf)-1]='\0';
	c=strchr(buf,' ');
	if(NULL==c)
	{
		if(!strcmp(buf,"pwd"))
		{
			return 1;
		}
	}
	c[0]='\0';
	c++;
	strcpy(o->order,buf);
	strcpy(o->buf,c);
	if(!strcmp(o->order,"cd"))
	{
		return 2;
	}
	return 10;
}
