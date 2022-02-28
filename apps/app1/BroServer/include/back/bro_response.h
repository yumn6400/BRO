#ifndef __BRO_RESPONSE__
#define __BRO_RESPONSE__ 9999
#include<bro.h>
#include<bro_cookie.h>
#include<bro_validator.h>
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
set<string> cookies;
int cookiesDataLength;
public:
Response();
~Response();
void setContentType(string contentType);
void addCookie(Cookie &cookie);
Response & operator<<(Cookie &cookie);
Response & operator<<(string content);
friend class HttpResponseUtility;
};
#endif