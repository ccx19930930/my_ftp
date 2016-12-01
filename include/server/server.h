#ifndef __SERVER_H__
#define __SERVER_H__

#include"head.h"

void *server(void *v);

void server_pwd(int sfd);
void server_cd(int sfd);
void server_ls(int sfd);
void server_mkdir(int sfd);
void server_remove(int sfd);
int server_puts(int sfd,file_serve_struct *f_puts);
void server_gets(int sfd,file_serve_struct *f_gets);


#endif
