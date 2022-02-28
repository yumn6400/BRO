#ifndef __BRO_COOKIE__
#define __BRO_COOKIE__ 9999
#include<bro.h>
#include<bro_gmt_date_time.h>
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
Cookie(string name,string value);
string getName();
string getValue();
void setExpiresOn(GMTDateTime &gmtDateTime);
string getExpiresOn();
void setMaxAgeInSeconds(int maxAgeInSeconds);
void setMaxAgeInMinutes(int maxAgeInMinutes);
void setMaxAgeInHours(int maxAgeInHours);
int getMaxAge();
void setDomain(string domain);
string getDomain();
void setPath(string path);
string getPath();
void setSecure(bool isSecure);
bool getSecure();
void setHttpOnly(bool isHttpOnly);
bool getHttpOnly();
string stringify();
};
#endif
