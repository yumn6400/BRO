#include<bro_application_level_container_dependent_function.h>
ApplicationLevelContainerDependentFunction::ApplicationLevelContainerDependentFunction(void(*mappingFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
this->mappingFunction=mappingFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
void ApplicationLevelContainerDependentFunction::doService(Request &request,Response &response)
{
this->mappingFunction(request,response,*p2ApplicationLevelContainer);
}