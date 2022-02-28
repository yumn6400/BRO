#include<bro.h>
bool Bro::isCHTML(const char *requestURI)
{
//a.chtml
int len=strlen(requestURI);
if(len<7)return false;
const char *ptr1=requestURI+len-6;
char lastPart[7]={".chtml"};
const char *ptr2=lastPart;
char a,b;
while(*ptr1&&*ptr2)
{
a=*ptr1;
b=*ptr2;
if(a>=97&&a<=122)a-=32;
if(b>=97&&b<=122)b-=32;
if(a!=b)return false;
ptr1++;
ptr2++;
}
return true;
}
Bro::Bro()
{
BroUtilities::loadMIMETypes(mimeTypes);
if(mimeTypes.size()==0)throw string("bro-data folder has been tempered with");
}
Bro::~Bro()
{
}
void Bro::setStaticResourcesFolder(string staticResourcesFolder)
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

void Bro::processCHTMLResource(int clientSocketDescriptor,const char *requestURI,Request &request)
{
if(this->staticResourcesFolder.length()==0)
{
//send back 404
return;
}
if(!FileSystemUtility::directoryExists(this->staticResourcesFolder.c_str()))
{
//send back 404 
return;
}
string resourcePath=this->staticResourcesFolder+string(requestURI);
if(!FileSystemUtility::fileExists(resourcePath.c_str()))
{
//send back 404
return;
}
TemplateEngine::processCHTMLFile(resourcePath.c_str(),request,clientSocketDescriptor);
}

bool Bro::serverStaticResource(int clientSocketDescriptor,const char *requestURI)
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
void Bro::addStartupService(int priorityNumber,void(*startupFunction)(void))
{
StartupFunction *sf;
sf=new SimpleStartupFunction(priorityNumber,startupFunction);
this->startupFunctions.push(sf);
}
void Bro::addStartupService(int priorityNumber,void(*startupFunction)(ApplicationLevelContainer &))
{
StartupFunction *sf;
sf=new ApplicationLevelContainerDependentStartupFunction(priorityNumber,startupFunction,&(applicationLevelContainer));
this->startupFunctions.push(sf);
}


void Bro::get(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new SimpleFunction(callBack);
urlMappings.insert(pair<string,URLMapping>(url,{__GET__,serviceFunction}));
}
}
void Bro::get(string url,void (*callBack)(Request &,Response &,ApplicationLevelContainer &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new ApplicationLevelContainerDependentFunction(callBack,&(this->applicationLevelContainer));
urlMappings.insert(pair<string,URLMapping>(url,{__GET__,serviceFunction}));
}
}
void Bro::post(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
ServiceFunction *serviceFunction;
serviceFunction=new SimpleFunction(callBack);
urlMappings.insert(pair<string,URLMapping>(url,{__POST__,serviceFunction}));
}
}
void Bro::listen(int portNumber,void (*callBack)(Error &))
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
cout<<requestBuffer<<endl;
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
while(requestBuffer[i]&&requestBuffer[i]!='\r'&&requestBuffer[i]!='\n')
{
i++;
}
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
int headerStartIndex=i;
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
if(isCHTML(requestURI))
{
map<string,string> headerFieldsMap;
HeaderUtility::parseHeader(requestBuffer+headerStartIndex,headerFieldsMap);
Request request(method,requestURI,httpVersion,dataInRequest,headerFieldsMap);
processCHTMLResource(clientSocketDescriptor,requestURI,request);
}
else if(!serverStaticResource(clientSocketDescriptor,requestURI))
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

map<string,string> headerFieldsMap;
HeaderUtility::parseHeader(requestBuffer+headerStartIndex,headerFieldsMap);

//code to parset the header and then the payload if exist ends here
Request request(method,requestURI,httpVersion,dataInRequest,headerFieldsMap);
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
//at place of requestURI,forwardTo
if(isCHTML(forwardTo.c_str()))
{
//Request request(method,requestURI,httpVersion,dataInRequest);
request.forwardTo(string(""));
processCHTMLResource(clientSocketDescriptor,forwardTo.c_str(),request);
}
else if(!serverStaticResource(clientSocketDescriptor,forwardTo.c_str()))
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
closesocket(clientSocketDescriptor);//done done
}//infinite loop ends here    
#ifdef _WIN32
WSACleanup();
#endif
}