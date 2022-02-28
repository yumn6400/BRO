#ifndef __BRO_STARTUP_FUNCTION__
#define __BRO_STARTUP_FUNCTION__ 9999
#include<bro.h>
class StartupFunction
{
public:
virtual int getPriorityNumber()=0;
virtual void run()=0;
};
#endif