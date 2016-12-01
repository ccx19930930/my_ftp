#include"func.h"

extern puser u;

void *pthread_puts(void *v)
{
	file_serve_struct *fss=(file_serve_struct*)v;
	p_file_queue pcur;
	data d;
	int ret;
	int total;
lable:
	while(1)
	{
		total=0;
		pthread_mutex_lock(&fss->mutex);
		if(fss->f.phead==fss->f.ptail)
		{
			pthread_cond_wait(&fss->cond,&fss->mutex);
		}
		file_dequeue(&fss->f,&pcur);
		pthread_mutex_unlock(&fss->mutex);

		send_n(pcur->sfd,(char*)&pcur->seek,sizeof(off_t));
		lseek(pcur->fd,pcur->seek,SEEK_SET);
		
		
		while(1)
		{
			memset(&d,0,sizeof(d));
			ret=recv_n(pcur->sfd,(char*)&d.len,4);
			if(-1==ret)
			{
				close(pcur->fd);
				close(pcur->sfd);
				memset(u+pcur->sfd,0,sizeof(user));
				free(pcur);
				goto lable;
			}	
			if(-2==d.len)
			{
				break;
			}
			else if(-1!=d.len)
			{
				ret=recv_n(pcur->sfd,d.buf,d.len);
				if(-1==ret)
				{
					close(pcur->fd);
					close(pcur->sfd);
					memset(u+pcur->sfd,0,sizeof(user));
					free(pcur);
					goto lable;
				}
				write(pcur->fd,d.buf,d.len);
			}else{
				close(pcur->fd);
				write(u[pcur->sfd].fdw,&pcur->sfd,sizeof(int));
				free(pcur);
				goto lable;
			}
			total+=d.len;
		}
		(pcur->seek)+=total;
		pthread_mutex_lock(&fss->mutex);
		file_enqueue(&fss->f,pcur);
		pthread_mutex_unlock(&fss->mutex);
	}
}
