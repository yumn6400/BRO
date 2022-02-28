#include<bro_request.h>
void Request::setCHTMLVariable(string name,string value)
{
varMap.insert({name,value}); //varMap.insert(pair<string,string>(name,value));
}
void Request::setCHTMLVariable(string name,Stringifyable *stringifyable)
{
varMap.insert({name,stringifyable->stringify()}); //varMap.insert(pair<string,string>(name,value));
}
void Request::setCHTMLVariable(string name,const char *ptr)
{
setCHTMLVariable(name,string(ptr));
}
void Request::setCHTMLVariable(string name,short int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,unsigned short int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,unsigned int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,unsigned long int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,long long int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,unsigned long long int value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,float value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,double value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,long double value)
{
varMap.insert({name,to_string(value)});
}
void Request::setCHTMLVariable(string name,char value)
{
string str;
str+=value;
varMap.insert({name,str});
}
void Request::setCHTMLVariable(string name,unsigned char value)
{
string str;
str+=value;
varMap.insert({name,str});
}
void Request::setCHTMLVariable(string name,bool value)
{
if(value)varMap.insert({name,string("true")});
else varMap.insert({name,string("false")});
}
bool Request::containsCHTMLVariable(string name)
{
return varMap.find(name)!=varMap.end();
}
string Request::getCHTMLVariable(string name)
{
auto i=varMap.find(name);
if(i==varMap.end())return string("");
else return i->second;
}
void Request::forwardTo(string _forwardTo)
{
this->_forwardTo=_forwardTo;
}
bool Request::isToBeForwarded()
{
return this->_forwardTo.length()>0;
}
string Request::forwardToWhichResource()
{
return this->_forwardTo;
}
Request::Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest,map<string,string> &headerFieldsMap):headerFieldsMap(headerFieldsMap)
{
this->method=method;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
if(dataInRequest!=NULL&&strcmp(this->method,"get")==0)
{
createDataMap(dataInRequest,dataMap);
}
}
void Request::createDataMap(char *str,map<string,string> &dataMap)
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
string Request::operator[](string key)
{
auto iterator=dataMap.find(key);
if(iterator==dataMap.end())return string("");
return iterator->second;
}
string Request::getCookieValue(string name)
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
void Request::getCookies(list<Cookie> &cookies)
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
void Request::getCookieNames(list<string> &cookieNames)
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