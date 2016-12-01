#include"func.h"

void send_user(int sfdw,puser p)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	iov[0].iov_base=p->user;
	iov[0].iov_len=32;
	iov[1].iov_base=inet_ntoa(p->ip);
	iov[1].iov_len=13;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	size_t len;
	len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=p->sfd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=sendmsg(sfdw,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}
}
void recv_user(int sfdr,pser s)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	iov[0].iov_base=s->user;
	iov[0].iov_len=32;
	iov[1].iov_base=s->ip;
	iov[1].iov_len=16;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	size_t len;
	len=CMSG_LEN(sizeof(int));
	cmsg=(struct cmsghdr*)calloc(1,len);
	cmsg->cmsg_len=len;
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret;
	ret=recvmsg(sfdr,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}
	s->sfd=*(int*)CMSG_DATA(cmsg);
}

