#ifndef __BRO_STARTUP_FUNCTION_COMPARATOR__
#define __BRO_STARTUP_FUNCTION_COMPARATOR__ 9999
#include<bro.h>
#include<bro_startup_function.h>
class StartupFunctionComparator
{
public:
int operator()(StartupFunction *e,StartupFunction *f);
};
#endif