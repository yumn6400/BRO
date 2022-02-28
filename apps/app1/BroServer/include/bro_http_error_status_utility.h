#ifndef __BRO_HTTP_ERROR_STATUS_UTILITY__
#define __BRO_HTTP_ERROR_STATUS_UTILITY__ 9999
#include<bro_global.h>
class HttpErrorStatusUtility
{
private:
HttpErrorStatusUtility();
public:
static void sendBadRequestError(int clientSocketDescriptor);
static void sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *httpVersion);
static void sendNotFoundError(int clientSocketDescriptor,char *requestURI);
static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method,char *requestURI);
};
#endif
