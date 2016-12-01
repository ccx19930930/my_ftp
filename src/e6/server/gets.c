#include"func.h"

void *pthread_gets(void *v)
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
		off_t k=lseek(pcur->fd,pcur->seek,SEEK_SET);
		while(total<pcur->num)
		{
			memset(&d,0,sizeof(d));
			d.len=read(pcur->fd,&d.buf,sizeof(d.buf));
			if(d.len<=0)
			{
				d.len=0;
				send_n(pcur->sfd,(char*)&d.len,4);
				close(pcur->fd);
				
				goto lable;
			}
			total+=d.len;
			ret=send_n(pcur->sfd,(char*)&d,d.len+4);
			if(-1==ret)
			{
				close(pcur->fd);
				goto lable;
			}
		}
		pcur->seek+=total;
		pthread_mutex_lock(&fss->mutex);
		file_enqueue(&fss->f,pcur);
		pthread_mutex_unlock(&fss->mutex);
	}
}
