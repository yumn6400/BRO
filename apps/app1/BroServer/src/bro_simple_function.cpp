#include<bro_simple_function.h>
SimpleFunction::SimpleFunction(void(*mappingFunction)(Request &,Response &))
{
this->mappingFunction=mappingFunction;
}
void SimpleFunction::doService(Request &request,Response &response)
{
this->mappingFunction(request,response);
}