#ifndef __BRO_SERVICE_FUNCTION__
#define __BRO_SERVICE_FUNCTION__ 9999
#include<bro.h>
#include<bro_request.h>
#include<bro_response.h>
class ServiceFunction
{
public:
virtual void doService(Request &,Response &)=0;
};
#endif