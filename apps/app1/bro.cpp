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
#include<set>
#ifdef _WIN32
#include<windows.h>
const char *PATH_SEPARATOR="\\";
#endif
#ifdef linux
const char *PATH_SEPARATOR="/";
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
using namespace std;
//Amit (The Bro Programmer)

#define _forward_(request,url) \
request.forwardTo(url); \
return;



enum __container_operation_failure_reason__{__KEY_EXISTS__,__KEY_DOES_NOT_EXIST__,__OUT_OF_MEMORY__,__VALUE_SIZE_MISMATCH__};

class Stringifyable
{
public:
virtual string stringify()=0;
};

class GMTDateTime
{
private:
struct tm gmtDateTime;
bool isValid;
public:
GMTDateTime() //to pick sys date time
{
time_t elapsedSeconds;
elapsedSeconds=time(NULL);//return number of seconds elapsed since 1 jan 1970
if(elapsedSeconds==-1)
{
this->isValid=false;
this->gmtDateTime.tm_mday=0;
this->gmtDateTime.tm_mon=0;
this->gmtDateTime.tm_year=0;
this->gmtDateTime.tm_hour=0;
this->gmtDateTime.tm_min=0;
this->gmtDateTime.tm_sec=0;
return;
}
struct tm *gmt=gmtime(&elapsedSeconds);
this->gmtDateTime=*gmt;
this->isValid=true;
}
//month(0-11)
//year(actual year-1900)
GMTDateTime(int day,int month,int year,int hour,int minute,int second)
{
struct tm tmptm;
tmptm.tm_mday=day;
tmptm.tm_mon=month;
tmptm.tm_year=year;
tmptm.tm_hour=hour;
tmptm.tm_min=minute;
tmptm.tm_sec=second;
if(mktime(&tmptm)==-1)//mktime will set wday part in structure
{
this->isValid=false;
this->gmtDateTime.tm_mday=0;
this->gmtDateTime.tm_mon=0;
this->gmtDateTime.tm_year=0;
this->gmtDateTime.tm_hour=0;
this->gmtDateTime.tm_min=0;
this->gmtDateTime.tm_sec=0;
return;
}
this->isValid=true;
this->gmtDateTime=tmptm;
}
int getDay()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_mday;
}
int getMonth() //return in form of 1 to 12
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_mon+1;
}
int getYear() //return actual year
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_year+1900;
}
int getHour()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_hour;
}
int getMinute()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_min;
}
int getSecond()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_sec;
}
int getWeekDay() //return sun to sat in form of 0 to 6
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_wday;
}
string stringify()
{
if(!this->isValid)return string("");
char buffer[80];
strftime(buffer,80,"%a,%d %b %Y %H:%M:%S GMT",&(this->gmtDateTime));
return string(buffer);
}
};
ostream & operator<<(ostream &out, GMTDateTime &gmtDateTime)
{
out<<gmtDateTime.stringify();
return out;
}

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
static bool createDirectory(const char *directoryName)
{
#ifdef _WIN32
return (mkdir(directoryName)==0);
#endif
#ifdef linux
return (mkdir(directoryName,S_IRWXU)==0);
#endif
}
static unsigned int getLastUpdatedTime(const char *fileName)
{
struct stat attributes;
stat(fileName,&attributes);
return attributes.st_mtime;
}
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

typedef enum _cookie_same_site_flag
{
COOKIE_SAME_SITE_FLAG_NONE,
COOKIE_SAME_SITE_FLAG_VAX,
COOKIE_SAME_SITE_FLAG_STRICT
}COOKIE_SAME_SITE_FLAG;

class Cookie
{
private:
string name;
string value;
string expiresOn;
long maxAge;
string domain;
string path;
bool isSecure;
bool isHttpOnly;
COOKIE_SAME_SITE_FLAG sameSiteFlag;
public:
Cookie(string name,string value)
{
this->name=name;
this->value=value;
this->expiresOn="";
this->maxAge=-1;
this->domain="";
this->path="";
this->isSecure=false;
this->isHttpOnly=false;
}
string getName()
{
return this->name;
}
string getValue()
{
return this->value;
}
void setExpiresOn(GMTDateTime &gmtDateTime)
{
this->expiresOn=gmtDateTime.stringify();
}
string getExpiresOn()
{
return this->expiresOn;
}
void setMaxAgeInSeconds(int maxAgeInSeconds)
{
this->maxAge=maxAgeInSeconds;
}
void setMaxAgeInMinutes(int maxAgeInMinutes)
{
this->maxAge=maxAgeInMinutes*60;
}
void setMaxAgeInHours(int maxAgeInHours)
{
this->maxAge=maxAgeInHours*60*60;
}
int getMaxAge()
{
return this->maxAge;
}
void setDomain(string domain)
{
this->domain=domain;
}
string getDomain()
{
return this->domain;
}
void setPath(string path)
{
this->path=path;
}
string getPath()
{
return this->path;
}
void setSecure(bool isSecure)
{
this->isSecure=isSecure;
}
bool getSecure()
{
return this->isSecure;
}
void setHttpOnly(bool isHttpOnly)
{
this->isHttpOnly=isHttpOnly;
}
bool getHttpOnly()
{
return this->isHttpOnly;
}
string stringify()
{
//write the necessary code to generate a string or
/*  
Note: 
if same site part is assigned None then secure needs to be added
Think about the __secure- or __Host- prefixed name part
*/
return "";//you will have to change this
}
};

class HeaderUtility
{
private:
HeaderUtility(){}
public:
static void parseHeader(const char *header,map<string,string> &headerFieldsMap)
{
const char *sp,*ep,*ptr,*colonPtr;
ptr=header;
while(*ptr)
{
sp=ptr;
colonPtr=NULL;
while(*ptr && *ptr!='\r' && *ptr!='\n')
{
if(colonPtr==NULL && *ptr==':')colonPtr=ptr;
ptr++;
}
ep=ptr-1;
if(*ptr== '\r')ptr+=2;
else ptr+=1;
if(sp==ptr || colonPtr==NULL) //really,this will never happen
{
ptr++;
continue;
}
string fieldName(sp,colonPtr-sp);
if(*(colonPtr+1)==' ')sp=colonPtr+2;
else sp=colonPtr+1;
string fieldValue(sp,ep-sp+1);
transform(fieldName.begin(),fieldName.end(),fieldName.begin(),::tolower);
cout<<"["<<fieldName<<"],["<<fieldValue<<"]"<<endl;
headerFieldsMap.insert({fieldName,fieldValue});
if(*ptr=='\r' && *(ptr+1)=='\n')break; //headers ends
}
}
};


class Request:public Container
{
private:
map<string,string>dataMap;
map<string,string> &headerFieldsMap;
char *method,*httpVersion;
const char *requestURI;
string _forwardTo;
map<string,string>varMap;
public:
void setCHTMLVariable(string name,string value)
{
varMap.insert({name,value}); //varMap.insert(pair<string,string>(name,value));
}
void setCHTMLVariable(string name,Stringifyable *stringifyable)
{
varMap.insert({name,stringifyable->stringify()}); //varMap.insert(pair<string,string>(name,value));
}
void setCHTMLVariable(string name,const char *ptr)
{
setCHTMLVariable(name,string(ptr));
}
void setCHTMLVariable(string name,short int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,unsigned short int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,unsigned int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,unsigned long int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,long long int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,unsigned long long int value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,float value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,double value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,long double value)
{
varMap.insert({name,to_string(value)});
}
void setCHTMLVariable(string name,char value)
{
string str;
str+=value;
varMap.insert({name,str});
}
void setCHTMLVariable(string name,unsigned char value)
{
string str;
str+=value;
varMap.insert({name,str});
}
void setCHTMLVariable(string name,bool value)
{
if(value)varMap.insert({name,string("true")});
else varMap.insert({name,string("false")});
}
bool containsCHTMLVariable(string name)
{
return varMap.find(name)!=varMap.end();
}
string getCHTMLVariable(string name)
{
auto i=varMap.find(name);
if(i==varMap.end())return string("");
else return i->second;
}
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
Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest,map<string,string> &headerFieldsMap):headerFieldsMap(headerFieldsMap)
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
public:
string getCookieValue(string name)
{
auto f=headerFieldsMap.find("cookie");
if(f==headerFieldsMap.end())return string("");
string cookieString=f->second;
const char *ptr,*nsp,*nep,*vsp,*vep;
ptr=cookieString.c_str();
while(*ptr)
{
if(*ptr==' ')ptr++;
if(*ptr=='\0')break;
nsp=ptr;
nep=ptr;
while(*nep && *nep!='=')nep++;
if(*nep=='\0')break; //this will never happen
vsp=nep+1;
vep=vsp;
while(*vep && *vep!=';')vep++;
nep--; // because nep is pointing to the block that contains =
vep--; //because vep is pointing to the block that contains ;
string cookieName(nsp,nep-nsp+1);
if(cookieName==name)return string(vsp,vep-vsp+1);
ptr=vep+2;
}
return string("");
}
void getCookies(list<Cookie> &cookies)
{
auto f=headerFieldsMap.find("cookie");
if(f==headerFieldsMap.end())return;
string cookieString=f->second;
const char *ptr,*nsp,*nep,*vsp,*vep;
ptr=cookieString.c_str();
while(*ptr)
{
if(*ptr==' ')ptr++;
if(*ptr=='\0')break;
nsp=ptr;
nep=ptr;
while(*nep && *nep!='=')nep++;
if(*nep=='\0')break; //this will never happen
vsp=nep+1;
vep=vsp;
while(*vep && *vep!=';')vep++;
nep--; // because nep is pointing to the block that contains =
vep--; //because vep is pointing to the block that contains ;
string cookieName(nsp,nep-nsp+1);
string cookieValue(vsp,vep-vsp+1);
Cookie cookie(cookieName,cookieValue);
cookies.push_back(cookie);
ptr=vep+2;
}
}
void getCookieNames(list<string> &cookieNames)
{
auto f=headerFieldsMap.find("cookie");
if(f==headerFieldsMap.end())return;
string cookieString=f->second;
const char *ptr,*nsp,*nep,*vsp,*vep;
ptr=cookieString.c_str();
while(*ptr)
{
if(*ptr==' ')ptr++;
if(*ptr=='\0')break;
nsp=ptr;
nep=ptr;
while(*nep && *nep!='=')nep++;
if(*nep=='\0')break; //this will never happen
vsp=nep+1;
vep=vsp;
while(*vep && *vep!=';')vep++;
nep--; // because nep is pointing to the block that contains =
vep--; //because vep is pointing to the block that contains ;
string cookieName(nsp,nep-nsp+1);
cookieNames.push_back(cookieName);
ptr=vep+2;
}
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
set<string> cookies;
int cookiesDataLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
this->cookiesDataLength=0;
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
void addCookie(Cookie &cookie)
{
string name=cookie.getName();
if(name.length()==0)return;
string value=cookie.getValue();
if(value.length()==0)return;
string cookieString=name+string("=")+value;
if(cookies.find(cookieString)!=cookies.end())return;
this->cookiesDataLength+=cookieString.length();
this->cookiesDataLength+=12;// "Set-Cookie: "
this->cookiesDataLength+=1; // "\n"
this->cookies.insert(cookieString);
}
Response & operator<<(Cookie &cookie)
{
this->addCookie(cookie);
return *this;
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
//char header[200];
char *header=new char[200+response.cookiesDataLength];
int contentLength=response.contentLength;
sprintf(header,"HTTP/1.1 200 OK \r\n Content-Type:text/html\r\n Content-Length:%d\r\n",contentLength);
for(string cookieString:response.cookies)
{
strcat(header,"Set-Cookie: ");
strcat(header,cookieString.c_str());
strcat(header,"\n");
}
strcat(header,"Connection: close\r\n\r\n");
send(clientSocketDescriptor,header,strlen(header),0);
header[strlen(header)]='\0';
auto contentIterator=response.content.begin();
while(contentIterator!=response.content.end())
{
string str=*contentIterator;
send(clientSocketDescriptor,str.c_str(),str.length(),0);
++contentIterator;
}
delete []header;
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


class TemplateEngine
{
private:
struct vmd
{
long start_position;
long end_position;
char var_name[256];
};
static void createVMDFileName(const char *chtmlFileName,char *vmdFileName)
{
char *dotPtr;
const char *ptr;
ptr=chtmlFileName;
while(*ptr!='\0')ptr++;
while(ptr>=chtmlFileName)
{
if(*ptr=='/')
{
chtmlFileName=ptr+1;
break;
}
ptr--;
}
for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
{
if(*chtmlFileName=='.')dotPtr=vmdFileName;
*vmdFileName=*chtmlFileName;
}
*vmdFileName='\0';
strcpy(dotPtr+1,"vmd");
}
static void createVMDFile(const char *chtmlFileName,const char *pathToVMDFile)
{
char  *start,*end,*ptr;
unsigned int x;
FILE *f;
int toRead;
char buffer[513];
long fileLength;
f=fopen(chtmlFileName,"rb");
if(f==NULL)
{
//code to send back 404 error
return;
}
FILE *vmdFile;
vmdFile=fopen(pathToVMDFile,"wb");
if(vmdFile==NULL)
{
//code to send back 404 error
fclose(f);
return;
}
fseek(f,0,SEEK_END);
fileLength=ftell(f);
rewind(f);
unsigned int sp,ep;
int ii;
struct vmd vmd_record;
x=0;
while(1)
{
if((fileLength-x)>256)toRead=256;
else toRead=fileLength-x;
fread(buffer,toRead,1,f);
buffer[toRead]='\0';
x=x+toRead;
//processing the content of buffer starts here
start=buffer;
while(1)
{
start=strstr(start,"${");
if(start==NULL)
{
if(x==fileLength)break;
if(buffer[toRead-1]=='$')
{
buffer[0]='$';
if((fileLength-x)>255)toRead=255;
else toRead=fileLength-x;
fread(buffer+1,toRead,1,f);
x=x+toRead;
buffer[1+toRead]='\0';
start=buffer;
continue;
}
else
{
break;
}
}
end=strstr(start+2,"}");
if(end!=NULL)
{
for(ptr=start+1;ptr<end;ptr++)
{
if(*ptr=='$'&&*(ptr+1)=='{')break;
}
if(*ptr=='$')start=ptr;
//write vmd record to the file starts here
for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
{
vmd_record.var_name[ii]=*ptr;
}
vmd_record.var_name[ii]='\0';
sp=(x-toRead)+(start-buffer);
ep=(x-toRead)+(end-buffer);
cout<<"1-->"<<sp<<","<<ep<<endl;
vmd_record.start_position=sp;
vmd_record.end_position=ep;
fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
//write vmd record to the file ends here
start=end+1;

continue;
}
//if } not found,then load the next chunk of 256 bytes and store
//it in buffer from index 256
if(x==fileLength)break;
if((fileLength-x)>256)toRead=256;
else toRead=fileLength-x;
fread(buffer+256,toRead,1,f);
buffer[toRead+256]='\0';
x=x+toRead;
end=strstr(buffer+256,"}");
if(end==NULL)break;
for(ptr=start+1;ptr<end;ptr++)
{
if(*ptr=='$'&&*(ptr+1)=='{')break;
}
if(*ptr=='$')start=ptr;
//write vmd record to the file starts here
for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
{
vmd_record.var_name[ii]=*ptr;
}
vmd_record.var_name[ii]='\0';
sp=(x-toRead)+(start-buffer)-256;
ep=(x-toRead)+(end-buffer)-256;
cout<<"2-->"<<sp<<","<<ep<<endl;
vmd_record.start_position=sp;
vmd_record.end_position=ep;
fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
strcpy(buffer,buffer+256);
end=end-256;
start=end+1;
}//while(1) ends
//processing the contents of the buffer ends here
if(x==fileLength)break;
}
fclose(f);
fclose(vmdFile);
}
public:
//more parameters related to other type of containers will be added later on
static void processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor)
{
if(!FileSystemUtility::directoryExists("vmd_files"))
{
if(!FileSystemUtility::createDirectory("vmd_files"))
{
//we will implement this part later on
//the code to calculate response size starts here
//the code to calculate response size ends here
}
}
char vmdFileName[257];
createVMDFileName(chtmlFileName,vmdFileName);
string folderName=string("vmd_files");
string pathToVMDFile=folderName+string(PATH_SEPARATOR)+string(vmdFileName);
bool generateVMDFile=false;
if(FileSystemUtility::fileExists(pathToVMDFile.c_str()))
{
if(FileSystemUtility::getLastUpdatedTime(chtmlFileName)>FileSystemUtility::getLastUpdatedTime(pathToVMDFile.c_str()))
{
generateVMDFile=true;
}
}
else
{
generateVMDFile=true;
}
if(generateVMDFile)
{
createVMDFile(chtmlFileName,pathToVMDFile.c_str());
}
//process the chtml file (pick up info from vmd file)
//code to calculate response size starts here
FILE *chtmlFile=fopen(chtmlFileName,"rb");
FILE *vmdFile=fopen(pathToVMDFile.c_str(),"rb");
fseek(chtmlFile,0,SEEK_END);
long fileLength=ftell(chtmlFile);
rewind(chtmlFile);
string data;
long responseSize=fileLength;
struct vmd vmdRecord;
while(1)
{
fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
if(feof(vmdFile))break;
responseSize=(responseSize-(vmdRecord.end_position-vmdRecord.start_position)+1);
data=request.getCHTMLVariable(vmdRecord.var_name);
responseSize=responseSize+data.length();
}
//code to calculate response size ends here
//code to process chtml file starts here
string mimeType;
mimeType=string("text/html");
char header[200];
sprintf(header,"HTTP/1.1 200 OK\r\n Content-Type:%s\r\n Content-Length:%ld\r\n Connection:close\r\n\r\n",mimeType.c_str(),responseSize);
send(clientSocketDescriptor,header,strlen(header),0);
long bytesLeftToRead;
int bytesToRead;
char buffer[4096];
bytesLeftToRead=fileLength;
rewind(vmdFile);
long tmpBytesLeftToRead;
long bytesProcessFromFile=0;
while(1)
{
fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
if(feof(vmdFile))break;
tmpBytesLeftToRead=vmdRecord.start_position-bytesProcessFromFile;
bytesToRead=4096;
while(tmpBytesLeftToRead>0)
{
if(tmpBytesLeftToRead<bytesToRead)bytesToRead=tmpBytesLeftToRead;
fread(buffer,bytesToRead,1,chtmlFile);
if(feof(chtmlFile))break;//this will not happen
//send(clientSocketDescriptor,buffer,bytesToRead,0);
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesProcessFromFile+=bytesToRead;
tmpBytesLeftToRead-=bytesToRead;
}//inner loop ends
fread(buffer,(vmdRecord.end_position-vmdRecord.start_position)+1,1,chtmlFile);
bytesProcessFromFile+=((vmdRecord.end_position-vmdRecord.start_position)+1);
if(request.containsCHTMLVariable(vmdRecord.var_name))
{
data=request.getCHTMLVariable(vmdRecord.var_name);
send(clientSocketDescriptor,data.c_str(),data.length(),0);
}
}//outer loop ends

//vmd file ends but there may be any content is chtmlFile
//God is ${great}.Ujjain is the city of ${Gods}.I live in Ujjain.
//here vmd file has 2 record after completion is does not send
//I live in Ujjain.

bytesLeftToRead-=bytesProcessFromFile;
bytesToRead=4096;
while(bytesLeftToRead>0)
{
if(bytesLeftToRead<bytesToRead)bytesToRead=bytesLeftToRead;
fread(buffer,bytesToRead,1,chtmlFile);
if(feof(chtmlFile))break; //this will never happen
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesLeftToRead=bytesLeftToRead-bytesToRead;
}

//code to process chtml file ends here
fclose(chtmlFile);
fclose(vmdFile);
//close(clientSocketDescriptor);
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
bool isCHTML(const char *requestURI)
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

void processCHTMLResource(int clientSocketDescriptor,const char *requestURI,Request &request)
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
};

//Bobby[the web application developer]
class Bulb:public Stringifyable
{
private:
int wattage;
int price;
public:
void setWattage(int wattage)
{
this->wattage=wattage;
}
int getWattage()
{
return this->wattage;
}
void setPrice(int price)
{
this->price=price;
}
int getPrice()
{
return this->price;
}
string stringify()
{
return string("Wattage is: ")+to_string(this->wattage)+string(", Price is: ")+to_string(this->price);
}
};
int main()
{
try
{
Bro bro;
bro.setStaticResourcesFolder("c:/bro/whatever");
bro.get("/slogan",[](Request &request,Response &response) void{
string slogan;
ifstream iFile("data/sofd.data");
string line;
while(true)
{
if(!getline(iFile,line))break;
if(slogan.length()>0)slogan+=string("<br>");
slogan+=line;
}
iFile.close();
cout<<"Slogon of the day: "<<slogan<<endl;
request.setCHTMLVariable("sloganOfTheDay",slogan);
request.setCHTMLVariable("abcd","Hello");
Bulb bulb;
bulb.setWattage(60);
bulb.setPrice(100);
request.setCHTMLVariable("Bulb",&bulb);
request.setCHTMLVariable("aa",100);
request.setCHTMLVariable("bb",'a');
request.setCHTMLVariable("cc",100.88);
_forward_(request,string("/wordsOfWisdom.chtml"));
});


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
cout<<request.getCookieValue("RollNumber")<<endl;
cout<<request.getCookieValue("Name")<<endl;
cout<<request.getCookieValue("Gender")<<endl;
list<string> l1;
request.getCookieNames(l1);
list<Cookie> l2;
request.getCookies(l2);
cout<<"***************"<<endl;
for(string s:l1)
{
cout<<s<<endl;
}
cout<<"***************"<<endl;
for(Cookie c:l2)
{
cout<<c.getName()<<","<<c.getValue()<<endl;
}



cout<<"/coolBro3 function got called"<<endl;
});

bro.get("/aRequest",[](Request &request,Response &response)void{
cout<<"Function against /aRequest got called"<<endl;
int x=100;
//100 as a score is generated according to application requirement
request.set("score",x,NULL,NULL);
cout<<"Now forwarding request for /bRequest"<<endl;
_forward_(request,string("/bRequest"));
});
bro.get("/bRequest",[](Request &request,Response &response)void{
cout<<"Function against /bRequest got called"<<endl;
int whatever;
request.get("score",&whatever,NULL,NULL);
cout<<"Fetched data form request object,which was named as score"<<endl;
response.setContentType("text/html");
Cookie cookie("Name","Rahul");
Cookie cookie1("RollNumber","101");
Cookie cookie2("Gender","Male");
response<<cookie<<cookie1<<cookie2;
response<<"<html><head></head><body>";
response<<"<br><a href=\"/coolBro1\">coolBro1</a></body></html>";
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
