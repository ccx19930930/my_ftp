#ifndef __ORDER_H__
#define __ORDER_H__

#include"head.h"

typedef struct
{
	char order[16];
	char buf[512];
}orders,*porders;

int order_ana(orders *o);

#endif
