#ifndef __BRO_SIMPLE_STARTUP_FUNCTION__
#define __BRO_SIMPLE_STARTUP_FUNCTION__ 9999
#include<bro_global.h>
#include<bro_startup_function.h>
class SimpleStartupFunction:public StartupFunction
{
private:
int priorityNumber;
void(*startupFunction)(void);
public:
SimpleStartupFunction(int priorityNumber,void(*startupFunction)(void));
int getPriorityNumber();
void run();
};
#endif