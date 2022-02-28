#ifndef __BRO_REQUEST__
#define __BRO_REQUEST__ 9999
#include<bro_global.h>
#include<bro_cookie.h>
#include<bro_utilities.h>
#include<bro_stringifyable.h>
#include<bro_container.h>
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
void setCHTMLVariable(string name,string value);
void setCHTMLVariable(string name,Stringifyable *stringifyable);
void setCHTMLVariable(string name,const char *ptr);
void setCHTMLVariable(string name,short int value);
void setCHTMLVariable(string name,unsigned short int value);
void setCHTMLVariable(string name,int value);
void setCHTMLVariable(string name,unsigned int value);
void setCHTMLVariable(string name,unsigned long int value);
void setCHTMLVariable(string name,long long int value);
void setCHTMLVariable(string name,unsigned long long int value);
void setCHTMLVariable(string name,float value);
void setCHTMLVariable(string name,double value);
void setCHTMLVariable(string name,long double value);
void setCHTMLVariable(string name,char value);
void setCHTMLVariable(string name,unsigned char value);
void setCHTMLVariable(string name,bool value);
bool containsCHTMLVariable(string name);
string getCHTMLVariable(string name);
void forwardTo(string _forwardTo);
private:
bool isToBeForwarded();
string forwardToWhichResource();
Request(char *method,const char *requestURI,char *httpVersion,char *dataInRequest,map<string,string> &headerFieldsMap);
void createDataMap(char *str,map<string,string> &dataMap);
string operator[](string key);
public:
string getCookieValue(string name);
void getCookies(list<Cookie> &cookies);
void getCookieNames(list<string> &cookieNames);
friend class Bro;
};
#endif