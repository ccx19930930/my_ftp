#include"func.h"

void serve_process(pfactory fac)
{
	factory_init(fac);
	factory_start(fac);
	while(1);
}
