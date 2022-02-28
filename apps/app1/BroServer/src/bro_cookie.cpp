#include<bro_cookie.h>
Cookie::Cookie(string name,string value)
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
string Cookie::getName()
{
return this->name;
}
string Cookie::getValue()
{
return this->value;
}
void Cookie::setExpiresOn(GMTDateTime &gmtDateTime)
{
this->expiresOn=gmtDateTime.stringify();
}
string Cookie::getExpiresOn()
{
return this->expiresOn;
}
void Cookie::setMaxAgeInSeconds(int maxAgeInSeconds)
{
this->maxAge=maxAgeInSeconds;
}
void Cookie::setMaxAgeInMinutes(int maxAgeInMinutes)
{
this->maxAge=maxAgeInMinutes*60;
}
void Cookie::setMaxAgeInHours(int maxAgeInHours)
{
this->maxAge=maxAgeInHours*60*60;
}
int Cookie::getMaxAge()
{
return this->maxAge;
}
void Cookie::setDomain(string domain)
{
this->domain=domain;
}
string Cookie::getDomain()
{
return this->domain;
}
void Cookie::setPath(string path)
{
this->path=path;
}
string Cookie::getPath()
{
return this->path;
}
void Cookie::setSecure(bool isSecure)
{
this->isSecure=isSecure;
}
bool Cookie::getSecure()
{
return this->isSecure;
}
void Cookie::setHttpOnly(bool isHttpOnly)
{
this->isHttpOnly=isHttpOnly;
}
bool Cookie::getHttpOnly()
{
return this->isHttpOnly;
}
string Cookie::stringify()
{
//write the necessary code to generate a string or
/*  
Note: 
if same site part is assigned None then secure needs to be added
Think about the __secure- or __Host- prefixed name part
*/
return "";//you will have to change this
}
