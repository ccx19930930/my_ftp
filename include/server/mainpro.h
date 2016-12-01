#ifndef __MAINPRO_H__
#define __MAINPRO_H__

#include"head.h"
#include"user.h"
#include"factory.h"

void main_process(pconf_struct conf,pmain_pth pmain);
int verify_process(user *u);

#endif
