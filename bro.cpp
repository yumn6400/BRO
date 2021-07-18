#include<iostream>
#include<forward_list>
#include<map>
#include<string.h>
#include<unistd.h>
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
using namespace std;
class StringUtility
{
private:
StringUtility(){}
public:
static void toLowerCase(char *str)
{
if(str==NULL)return;
for(;*str;str++)if(*str>=65&&*str<=90)*str+=32;
}
};
class HttpErrorStatusUtility
{
private:
HttpErrorStatusUtility(){}
public:
static void sendBadRequestError(int clientSocketDescriptor)
{
//will complete later on
}
static void sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *httpVersion)
{
//will complete later on
}
static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
{
//we wil optimise this code later on
char content[1000];
char header[200];
char response[1200];
sprintf(content,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>Whatever</title></head><body>Requested Resource [%s] Not Found </body>",requestURI);
int contentLength=strlen(content);
sprintf(header,"HTTP/1.1 404 Not Found\r\n Content-Type:text/html\r\n Content-Length:%d\r\n Connection:close\r\n\r\n",contentLength);
strcpy(response,header);
strcat(response,content);
send(clientSocketDescriptor,response,strlen(response),0);
}
static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method,char *requestURI)
{
//will complete later on
}
};

class Validator
{
private:
Validator()
{

}
public:
static bool isValidMIMEType(string &mimeType)
{
//right now do nothing 
return true;
}
static bool isValidPath(string &path)
{
//right now do nothing 
return true;
}
static bool isValidURLFormat(string &url)
{
//right now do nothing 
return true;
}
};
class Error
{
private:
string error;
public:
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};
class Request
{
private:
char *method,*requestURI,*httpVersion;
public:
Request(char *method,char *requestURI,char *httpVersion)
{
this->method=method;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
}
friend class Bro;
};
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{
// not yet decided
}
void setContentType(string contentType)
{
if(Validator::isValidMIMEType(contentType))
{
this->contentType=contentType;
}
}
Response & operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
friend class HttpResponseUtility;
};
class HttpResponseUtility
{
private:
HttpResponseUtility(){}
public:
static void sendResponse(int clientSocketDescriptor,Response &response)
{
char header[200];
int contentLength=response.contentLength;
sprintf(header,"HTTP/1.1 200 OK \r\n Content-Type:text/html\r\n Content-Length:%d\r\n Connection:close\r\n\r\n",contentLength);
send(clientSocketDescriptor,header,strlen(header),0);
auto contentIterator=response.content.begin();
while(contentIterator!=response.content.end())
{
string str=*contentIterator;
send(clientSocketDescriptor,str.c_str(),str.length(),0);
++contentIterator;
}
}

};



enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__HEAD__,__OPTIONS__,__CONNECT__,__TRACE__};
typedef struct __url__mapping
{
__request_method__ requestMethod;
void(*mappingFunction)(Request &,Response &);
}URLMapping;
class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping>urlMapping;
public:
Bro()
{
}
~Bro()
{
}
void setStaticResourcesFolder(string staticResourceFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder;
}
else
{
//not yet decided
}
}
void get(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
urlMapping.insert(pair<string,URLMapping>(url,{__GET__,callBack}));
}
}
void listen(int portNumber,void (*callBack)(Error &))
{
int clientSocketDescriptor,serverSocketDescriptor;
struct sockaddr_in serverSocketInformation,clientSocketInformation;
char requestBuffer[4097];
int requestLength;
#ifdef _WIN32
WSAData wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);//socket Library initialized
#endif
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to create a socket");
callBack(error);
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
#ifdef linux
close(serverSocketDescriptor);
#endif
#ifdef _WIN32
closesocket(serverSocketDescriptor);
WSACleanup();
#endif
char x[101];
sprintf(x,"Unable to bind to port number :%d\n",portNumber);
Error error(x);
callBack(error);
return;
}
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
#ifdef linux
close(serverSocketDescriptor);
#endif
#ifdef _WIN32
closesocket(serverSocketDescriptor);
WSACleanup();
#endif
Error error("Unable to accept client connection");
callBack(error);
return;
}
Error error("");
callBack(error);
#ifdef _WIN32
int len=sizeof(clientSocketInformation);
#endif
#ifdef linux
socklen_t len=sizeof(clientSocketInformation);
#endif
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
//Not yet decided
}
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0||requestLength==-1)
{
close(clientSocketDescriptor);
continue;
}
int i;
char *method,*requestURI,*httpVersion;
requestBuffer[requestLength]='\0';
//code to parse the first line of the http request starts here
// GET \ HTTP 1.1\r\n
// GET \pqr.html HTTP 1.1\r\n
method=requestBuffer;
i=0;
while(requestBuffer[i]&&requestBuffer[i]!=' ')i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]=='\0'||requestBuffer[i]==' ')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
StringUtility::toLowerCase(method);
if(!(strcmp(method,"get")==0||
strcmp(method,"post")==0||
strcmp(method,"put")==0||
strcmp(method,"delete")==0||
strcmp(method,"head")==0||
strcmp(method,"options")==0||
strcmp(method,"connect")==0||
strcmp(method,"trace")==0))
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestURI=requestBuffer+i;
while(requestBuffer[i]&&requestBuffer[i]!=' ')i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' '||requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
httpVersion=requestBuffer+i;
while(requestBuffer[i]&&requestBuffer[i]!='\r'&&requestBuffer[i]!='\n')i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r'&&requestBuffer[i+1]!='\n')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r')
{
requestBuffer[i]='\0';
i=i+2;
}
else
{
requestBuffer[i]='\0';
i=i+1;
}
StringUtility::toLowerCase(httpVersion);
if(strcmp(httpVersion,"http/1.1")!=0)
{
HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
close(clientSocketDescriptor);
continue;
}
auto urlMappingsIterator=urlMapping.find(requestURI);
if(urlMappingsIterator==urlMapping.end())
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
close(clientSocketDescriptor);
continue;
}
URLMapping urlMapping=urlMappingsIterator->second;
if(urlMapping.requestMethod==__GET__&&strcmp(method,"get")!=0)
{
HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
close(clientSocketDescriptor);
continue;
}
//code to parse the header and then the payload if exist starts here
//code to parset the header and then the payload if exist ends here
Request request(method,requestURI,httpVersion);
Response response;
urlMapping.mappingFunction(request,response);
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
closesocket(clientSocketDescriptor);
}//infinite loop ends here    


#ifdef _WIN32
WSACleanup();
#endif
}
};
int main()
{
Bro bro;
bro.setStaticResourcesFolder("whatever");
bro.get("/",[](Request &request,Response &response) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Whatever</title>
<body>
<h1>Welcome</h1>
<h3>Administration</h3>
<a href='getCustomers'>Customers List</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/getCustomers",[](Request &request,Response &response)void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title> Whatever</title>
</head>
<body>
<h1> List of customer</h1>
<ul>
<li>Ramesh</li>
<li>Suresh</li>
<li>Mohan</li>
</ul>
<a href='/'>Home </a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.listen(6060,[](Error &error)void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro HTTP server is ready to accept request on port 6060"<<endl;
});
return 0;
}