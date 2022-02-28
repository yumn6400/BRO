#include<bro_http_error_status_utility.h>
HttpErrorStatusUtility::HttpErrorStatusUtility()
{
}
void HttpErrorStatusUtility::sendBadRequestError(int clientSocketDescriptor)
{
//will complete later on
}
void HttpErrorStatusUtility::sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *httpVersion)
{
//will complete later on
}
void HttpErrorStatusUtility::sendNotFoundError(int clientSocketDescriptor,char *requestURI)
{
//we wil optimise this code later on
char content[1000];
char header[200];
char response[1200];
sprintf(content,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>Whatever</title></head><body>Requested Resource [%s] Not Found </body>",requestURI);
int contentLength=strlen(content);
sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\r\nContent-Length:%d\r\nConnection:close\r\n\r\n",contentLength);
strcpy(response,header);
strcat(response,content);
send(clientSocketDescriptor,response,strlen(response),0);
}
void HttpErrorStatusUtility::sendMethodNotAllowedError(int clientSocketDescriptor,char *method,char *requestURI)
{
//will complete later on
}
