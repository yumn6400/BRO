#ifndef __BRO_SIMPLE_FUNCTION__
#define __BRO_SIMPLE_FUNCTION__ 9999
#include<bro.h>
#include<bro_request.h>
#include<bro_response.h>
#include<bro_service_function.h>
typedef struct __url__mapping
{
__request_method__ requestMethod;
ServiceFunction *ServiceFunction;
}URLMapping;
class SimpleFunction: public ServiceFunction
{
private:
void(*mappingFunction)(Request &,Response &);
public:
SimpleFunction(void(*mappingFunction)(Request &,Response &));
void doService(Request &request,Response &response);
};
#endif