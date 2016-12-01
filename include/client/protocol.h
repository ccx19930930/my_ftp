#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

typedef struct
{
	unsigned int len;
	char buf[1024];
}data;

typedef struct
{
	char md5[40];
	off_t size;
}file_stat;


int recv_protocol(int sfd,data *d);
int send_protocol(int sfd,data *d);
int send_n(int sfd,char *p,int len);
int recv_n(int sfd,char *p,int len);

#endif
