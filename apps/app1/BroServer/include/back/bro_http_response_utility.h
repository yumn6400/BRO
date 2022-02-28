#ifndef __BRO_HTTP_RESPONSE_UTILITY__
#define __BRO_HTTP_RESPONSE_UTILITY__ 9999
#include<bro.h>
#include<bro_response.h>
#include<bro_cookie.h>
class HttpResponseUtility
{
private:
HttpResponseUtility();
public:
static void sendResponse(int clientSocketDescriptor,Response &response);
};
#endif
