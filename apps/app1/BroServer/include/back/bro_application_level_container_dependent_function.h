#ifndef __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_FUNCTION__
#define __BRO_APPLICATION_LEVEL_CONTAINER_DEPENDENT_FUNCTION__ 9999
#include<bro.h>
#include<bro_service_function.h>
#include<bro_response.h>
#include<bro_application_level_container.h>
class ApplicationLevelContainerDependentFunction:public ServiceFunction
{
private:
void(*mappingFunction)(Request &,Response &,ApplicationLevelContainer &);
ApplicationLevelContainer *p2ApplicationLevelContainer;
public:
ApplicationLevelContainerDependentFunction(void(*mappingFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer);
void doService(Request &request,Response &response);
};
#endif