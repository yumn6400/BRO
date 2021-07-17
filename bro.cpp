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
};
class Bro
{
private:
string staticResourcesFolder;
map<string,void(*)(Request &,Response &)>urlMapping;
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
urlMapping.insert(pair<string,void(*)(Request &,Response &)>(url,callBack));
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

forward_list<string> requestBufferDS;
forward_list<string>::iterator requestBufferDSIterator;
requestBufferDSIterator=requestBufferDS.before_begin();
int requestBufferDSSize=0;
int requestDataCount=0;
while(1)
{
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0)break;
requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
requestBufferDSSize++;
requestDataCount+=requestLength;
}
char *requestData=new char[requestDataCount+1];
char *p=requestData;
const char *q;
requestBufferDSIterator=requestBufferDS.begin();
while(requestBufferDSIterator!=requestBufferDS.end())
{
q=(*requestBufferDSIterator).c_str();
while(*q)
{
*p=*q;
p++;
q++;
}
++requestBufferDSIterator;
}
*p='\0';
requestBufferDS.clear();

printf("-------------request data begin --------------\n");
printf("%s\n",requestData);
printf("---------------request data ends -----------\n");
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