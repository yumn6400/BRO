#include<iostream>
#include<bits/stdc++.h>
#include<forward_list>
#include<map>
#include<stdio.h>
#include<queue>
#include<vector>
#include<sys/stat.h>
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
//Amit (The Bro Programmer)
#define _forward_(request,url) \
request.forwardTo(url); \
return;


enum __container_operation_failure_reason__{__KEY_EXISTS__,__KEY_DOES_NOT_EXIST__,__OUT_OF_MEMORY__,__VALUE_SIZE_MISMATCH__};
class Container
{
private:
typedef struct _bag
{
void *ptr;
int size;
}Bag;
map<string,Bag> dataSet;
public:
template<class T>
void set(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto iterator=dataSet.find(keyName);
if(iterator!=dataSet.end())
{
if(reason)*reason={__KEY_EXISTS__};
if(success) *success=false;
return;
}
void *ptr;
ptr=malloc(sizeof(T));
if(ptr==NULL)
{
if(reason) *reason={__OUT_OF_MEMORY__};
if(success) *success=false;
return;
}
memcpy(ptr,&value,sizeof(T));
dataSet.insert(pair<string,Bag>(keyName,{ptr,sizeof(T)}));
}
template<class T>
void get(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto dataSetIterator=dataSet.find(keyName);
if(dataSetIterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success)*success=false;
return;
}
Bag bag;
bag=dataSetIterator->second;
if(bag.size!=sizeof(*value))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success)*success=false;
return;
}
memcpy(value,bag.ptr,sizeof(*value));
if(success)*success=true;
}
template<class T>
void remove(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto dataSetIterator=dataSet.find(keyName);
if(dataSetIterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success)*success=false;
return;
}
Bag bag;
bag=dataSetIterator->second;
if(bag.size!=sizeof(*value))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success)*success=false;
return;
}
memcpy(value,bag.ptr,sizeof(*value));
free(bag.ptr);//To release memory allocated by the Bro server programmer
if(success)*success=true;
}
bool contains(string keyName)
{
auto iterator=this->dataSet.find(keyName);
return iterator!=this->dataSet.end();
}
};

class ApplicationLevelContainer :public Container
{

};

class BroUtilities
{
private:
BroUtilities(){}
public:
static bool isHexChar(int w)
{
if(w>=48&&w<=57)return true;
if(w>='a'&&w<='f')return true;
if(w>='A'&&w<='F')return true;
return false;
}
static void decode(char *encodedString,char *decodedString)
{
char *ptr=encodedString;
char *d=decodedString;
int i,m;
i=0;
while(*ptr)
{
if(*ptr=='+')
{
d[i]=' ';
i++;
ptr++;
continue;
}
if(*ptr!='%')
{
d[i]=*ptr;
i++;
ptr++;
continue;
}
ptr++;
if(isHexChar(*ptr)&&(isHexChar(*(ptr+1))))
{
sscanf(ptr,"%2x",&m);
d[i]=m;
i++;
ptr+=2;
}
else
{
i=0;
break;
}
}
d[i]='\0';
}
static void loadMIMETypes(map<string,string> &mimeTypes)
{
FILE *file=fopen("C:/bro/apps/app1/bro-data/mime.types","r");//open in r mode due to text file
if(file==NULL)return;
char *mimeType,*extension;
char line[200];
int x;
while(true)
{
fgets(line,200,file);
if(feof(file))break;
if(line[0]=='#')continue;
//logic to remove \r\n from the end of the string start here
x=strlen(line)-1;
while(true)
{
if(line[x]=='\r'||line[x]=='\n')
{
line[x]='\0';
x--;
}
else 
{
break;
}
}
//logic to remove \r\n from the end of the string end here
mimeType= &line[0];
for(x=0;line[x]!='\t';x++);
line[x]='\0';
x++;
while(line[x]=='\t')x++;
while(true)
{
extension= &line[x];
while(line[x]!='\0'&&line[x]!=' ')x++;
if(line[x]=='\0')
{
mimeTypes.insert(pair<string,string>(string(extension),string(mimeType)));
break;
}
else
{
line[x]='\0';
mimeTypes.insert(pair<string,string>(string(extension),string(mimeType)));
x++;
}
}
}//outer while loop ends
fclose(file);
}
};
class FileSystemUtility
{
private:
FileSystemUtility(){}
public:
static bool fileExists(const char *path)
{
struct stat s;
int x;
x=stat(path,&s);
if(x!=0)return false;
if(s.st_mode&S_IFDIR)return false;
return true;
}
static bool directoryExists(const char *path)
{
struct stat s;
int x=stat(path,&s);
if(x!=0)return false;
if(s.st_mode&S_IFDIR)return true;
return false;
}
static string getFileExtension(const char *path)
{
int x;
x=strlen(path)-1;
while(x>=0&&path[x]!='.')x--;
if(x==-1||path[x]!='.')return string("");
return string(path+(x+1));
}
};
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
sprintf(header,"HTTP/1.1 404 Not Found\r\nContent-Type:text/html\r\nContent-Length:%d\r\nConnection:close\r\n\r\n",contentLength);
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
return FileSystemUtility::directoryExists(path.c_str());
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
map<string,string>dataMap;
char *method,*requestURI,*httpVersion;
string _forwardTo;
public:
void forwardTo(string _forwardTo)
{
this->_forwardTo=_forwardTo;
}
private:
bool isToBeForwarded()
{
return this->_forwardTo.length()>0;
}
string forwardToWhichResource()
{
return this->_forwardTo;
}
Request(char *method,char *requestURI,char *httpVersion,char *dataInRequest)
{
this->method=method;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
if(dataInRequest!=NULL&&strcmp(this->method,"get")==0)
{
createDataMap(dataInRequest,dataMap);
}
}
void createDataMap(char *str,map<string,string> &dataMap)
{
char *ptr1,*ptr2;
int keyLength,valueLength;
char *decoded;
ptr1=str;
ptr2=str;
while(1)
{
while(*ptr2!='\0'&&*ptr2!='=')ptr2++;
if(*ptr2=='\0')return;
*ptr2='\0';
keyLength=ptr2-ptr1;
decoded=new char[keyLength+1];
BroUtilities::decode(ptr1,decoded);
string key=string(decoded);
delete [] decoded;
ptr1=ptr2+1;
ptr2=ptr1;
while(*ptr2!='\0'&&*ptr2!='&')ptr2++;
if(*ptr2=='\0')
{
valueLength=ptr2-ptr1;
decoded=new char[keyLength+1];
BroUtilities::decode(ptr1,decoded);
dataMap.insert(pair<string,string>(key,decoded));
delete [] decoded;
break;
}
else
{
*ptr2='\0';
valueLength=ptr2-ptr1;
decoded=new char[keyLength+1];
BroUtilities::decode(ptr1,decoded);
dataMap.insert(pair<string,string>(key,decoded));
delete [] decoded;
ptr1=ptr2+1;
ptr2=ptr1;
}
}
}
string operator[](string key)
{
auto iterator=dataMap.find(key);
if(iterator==dataMap.end())return string("");
return iterator->second;
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
class ServiceFunction
{
public:
virtual void doService(Request &,Response &)=0;
};

class StartupFunction
{
public:
virtual int getPriorityNumber()=0;
virtual void run()=0;
};

typedef struct __url__mapping
{
__request_method__ requestMethod;
ServiceFunction *ServiceFunction;
}URLMapping;

class SimpleFunction:public ServiceFunction
{
private:
void(*mappingFunction)(Request &,Response &);
public:
SimpleFunction(void(*mappingFunction)(Request &,Response &))
{
this->mappingFunction=mappingFunction;
}
void doService(Request &request,Response &response)
{
this->mappingFunction(request,response);
}
};
class ApplicationLevelContainerDependentFunction:public ServiceFunction
{
private:
void(*mappingFunction)(Request &,Response &,ApplicationLevelContainer &);
ApplicationLevelContainer *p2ApplicationLevelContainer;
public:
ApplicationLevelContainerDependentFunction(void(*mappingFunction)(Request &,Response &,ApplicationLevelContainer &),ApplicationLevelContainer *p2ApplicationLevelContainer)
{
this->mappingFunction=mappingFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
void doService(Request &request,Response &response)
{
this->mappingFunction(request,response,*p2ApplicationLevelContainer);
}
};
class SimpleStartupFunction:public StartupFunction
{
private:
int priorityNumber;
void(*startupFunction)(void);
public:
SimpleStartupFunction(int priorityNumber,void(*startupFunction)(void))
{
this->priorityNumber=priorityNumber;
this->startupFunction=startupFunction;
}
int getPriorityNumber()
{
return this->priorityNumber;
}
void run()
{
startupFunction();
}
};
class ApplicationLevelContainerDependentStartupFunction:public StartupFunction
{
private:
int priorityNumber;
void(*startupFunction)(ApplicationLevelContainer &);
ApplicationLevelContainer *p2ApplicationLevelContainer;
public:
ApplicationLevelContainerDependentStartupFunction(int priorityNumber,void(*startupFunction)(ApplicationLevelContainer &),ApplicationLevelContainer *applicationLevelContainer)
{
this->priorityNumber=priorityNumber;
this->startupFunction=startupFunction;
this->p2ApplicationLevelContainer=p2ApplicationLevelContainer;
}
int getPriorityNumber()
{
return this->priorityNumber;
}
void run()
{
startupFunction(*p2ApplicationLevelContainer);
}
};
class StartupFunctionComparator
{
public:
int operator()(StartupFunction *e,StartupFunction *f)
{
return !(e->getPriorityNumber()<f->getPriorityNumber());
}
};


class Bro
{
private:
string staticResourcesFolder;
map<string,URLMapping>urlMappings;
map<string,string>mimeTypes;
priority_queue<StartupFunction *,vector<StartupFunction *>,StartupFunctionComparator> startupFunctions;
ApplicationLevelContainer applicationLevelContainer;
public:
Bro()
{
BroUtilities::loadMIMETypes(mimeTypes);
if(mimeTypes.size()==0)throw string("bro-data folder has been tempered with");
}
~Bro()
{
}
void setStaticResourcesFolder(string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder; 
}
else
{
string exception=string("Invalid static resource folder path:")+staticResourcesFolder;
throw exception;
}
}
bool serverStaticResource(int clientSocketDescriptor,const char *requestURI)
{
if(this->staticResourcesFolder.length()==0)return false;
if(!(FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str())))return false;
string resourcePath=this->staticResourcesFolder+string(requestURI);
if(!(FileSystemUtility::fileExists(resourcePath.c_str())))return false;
FILE *file=fopen(resourcePath.c_str(),"rb");
if(file==NULL)return false;
long fileSize;
fseek(file,0,SEEK_END);
fileSize=ftell(file);
if(fileSize==0)
{
fclose(file);
return false;
}
rewind(file);//To move the internal file pointer to the start of the file
string extension,mimeType;
extension=FileSystemUtility::getFileExtension(resourcePath.c_str());
if(extension.length()>0)
{
transform(extension.begin(),extension.end(),extension.begin(),::tolower);
auto mimeTypesIterator=mimeTypes.find(extension);
if(mimeTypesIterator!=mimeTypes.end())
{
mimeType=mimeTypesIterator->second;
}
else
{
mimeType=string("text/html");
}
}
else
{
mimeType=string("text/html");
}
char header[200];
sprintf(header,"HTTP/1.1 200 OK\r\nContent-Type:%s\r\nContent-Length:%d\r\nConnection:close\r\n\r\n",mimeType.c_str(),fileSize);
send(clientSocketDescriptor,header,strlen(header),0);
char buffer[4096];
long bytesLeftToRead=fileSize;
int bytesToRead=4096;
while(bytesLeftToRead>0)
{
if(bytesLeftToRead<bytesToRead)bytesToRead=bytesLeftToRead;
fread(buffer,bytesToRead,1,file);
if(feof(file))break; //this won't happen in our case
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesLeftToRead=bytesLeftToRead-bytesToRead;
}
fclose(file);
return true;
}
void addStartupService(int priorityNumber,void(*startupFunction)(void))
{
StartupFunction *sf;
sf=new SimpleStartupFunction(priorityNumber,startupFunction);
this->startupFunctions.push(sf);
}
void addStartupService(int priorityNumber,void(*startupFunction)(ApplicationLevelContainer &))
{
StartupFunction *sf;
sf=new ApplicationLevelContainerDependentStartupFunction(priorityNumber,startupFunction,&(applicationLevelContainer));
this->startupFunctions.push(sf);
}


void get(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new SimpleFunction(callBack);
urlMappings.insert(pair<string,URLMapping>(url,{__GET__,serviceFunction}));
}
}
void get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new ApplicationLevelContainerDependentFunction(callBack,&(this->applicationLevelContainer));
urlMappings.insert(pair<string,URLMapping>(url,{__GET__,serviceFunction}));
}
}
void post(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new SimpleFunction(callBack);
urlMappings.insert(pair<string,URLMapping>(url,{__POST__,serviceFunction}));
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
StartupFunction *startupFunction;
while(!startupFunctions.empty())
{
startupFunction=startupFunctions.top();
startupFunctions.pop();
startupFunction->run();
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
char *method,*requestURI,*httpVersion,*dataInRequest;
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
dataInRequest=NULL;
i=0;
while(requestURI[i]!='\0'&&requestURI[i]!='?')i++;
if(requestURI[i]=='?')
{
requestURI[i]='\0';
dataInRequest=requestURI+i+1;
}
auto urlMappingsIterator=urlMappings.find(requestURI);
if(urlMappingsIterator==urlMappings.end())
{
if(!serverStaticResource(clientSocketDescriptor,requestURI))
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
}
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
Request request(method,requestURI,httpVersion,dataInRequest);
while(true)
{
Response response;
urlMapping.ServiceFunction->doService(request,response);
if(!request.isToBeForwarded())
{
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
break;
}
string forwardTo=request.forwardToWhichResource();
urlMappingsIterator=urlMappings.find(forwardTo);
if(urlMappingsIterator==urlMappings.end())
{
if(!serverStaticResource(clientSocketDescriptor,forwardTo.c_str()))
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
}
break;
}
urlMapping=urlMappingsIterator->second;
if(urlMapping.requestMethod==__GET__&&strcmp(method,"get")!=0)
{
HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
break;
}
request.forwardTo(string(""));
}//loop ends
closesocket(clientSocketDescriptor);
}//infinite loop ends here    
#ifdef _WIN32
WSACleanup();
#endif
}
};
int main()
{
try
{
Bro bro;
bro.setStaticResourcesFolder("c:/bro/whatever");
bro.addStartupService(3,[](){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 3"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.addStartupService(1,[](){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 1"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.addStartupService(2,[](ApplicationLevelContainer &applicationLevelContainer){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 2"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.get("/",[](Request &request,Response &response) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Bro test cases</title>
<body>
<h1>Welcome</h1>
<br/><br/>
<a href='/coolBro1'>First Film</a><br/>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/coolBro1",[](Request &request,Response &response) void{
cout<<"/coolBro1 function got called"<<endl;
_forward_(request,string("/coolBro2"));
cout<<"Code after forwarding function"<<endl;
});
bro.get("/coolBro2",[](Request &request,Response &response) void{
cout<<"/coolBro2 function got called"<<endl;
_forward_(request,string("/coolBro3"));
});
bro.get("/coolBro3",[](Request &request,Response &response) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Bro test cases</title>
<body>
<h1>Welcome</h1>
<br/><br/>
<h1> CoolBro3 function got called</h1>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
cout<<"/coolBro3 function got called"<<endl;
});
bro.listen(6060,[](Error &error)void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro HTTP server is ready to accept request on port 6060"<<endl;
});
}catch(string exception)
{
cout<<exception<<endl;
}
return 0;
}
