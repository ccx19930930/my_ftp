#ifndef __FUNC_H__
#define __FUNC_H__

#include"head.h"
#include"main.h"
#include"server.h"
#include"file.h"
#include"factory.h"

//factory.c
void factory_init(pfactory);
void factory_start(pfactory);

//verify_process.c
void verify_process();

//server_process.c
void serve_process();

//server_thread.c
void *ls_thread(void *);
void *cd_thread(void *);
void *pdw_thread(void *);
void *puts_thread(void *);
void *gets_thread(void *);
void *remove_thread(void *);



#endif
