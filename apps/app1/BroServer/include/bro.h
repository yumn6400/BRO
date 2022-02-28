#ifndef __BRO__
#define __BRO__ 9999
#include<bro.h>
#include<bro_service_function.h>
#include<bro_startup_function.h>
#include<bro_startup_function_comparator.h>
#include<bro_template_engine.h>
#include<bro_container.h>
#include<bro_application_level_container.h>
#include<bro_application_level_container_dependent_function.h>
#include<bro_application_level_container_dependent_startup_function.h>
#include<bro_error.h>
#include<bro_simple_startup_function.h>
#include<bro_http_error_status_utility.h>
#include<bro_http_response_utility.h>
#include<bro_header_utility.h>
#include<bro_string_utility.h>
#include<bro_simple_function.h>
class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping>urlMappings;
map<string,string>mimeTypes;
priority_queue<StartupFunction *,vector<StartupFunction *>,StartupFunctionComparator> startupFunctions;
ApplicationLevelContainer applicationLevelContainer;
bool isCHTML(const char *requestURI);
public:
Bro();
~Bro();
void setStaticResourcesFolder(string staticResourcesFolder);
void processCHTMLResource(int clientSocketDescriptor,const char *requestURI,Request &request);
bool serverStaticResource(int clientSocketDescriptor,const char *requestURI);
void addStartupService(int priorityNumber,void(*startupFunction)(void));
void addStartupService(int priorityNumber,void(*startupFunction)(ApplicationLevelContainer &));
void get(string url,void (*callBack)(Request &,Response &));
void get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &));
void post(string url,void (*callBack)(Request &,Response &));
void listen(int portNumber,void (*callBack)(Error &));
};
#endif