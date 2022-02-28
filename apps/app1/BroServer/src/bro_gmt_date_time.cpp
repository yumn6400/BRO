
#include<bro_gmt_date_time.h>
GMTDateTime::GMTDateTime() //to pick sys date time
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
GMTDateTime::GMTDateTime(int day,int month,int year,int hour,int minute,int second)
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
int GMTDateTime::getDay()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_mday;
}
int GMTDateTime::getMonth() //return in form of 1 to 12
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_mon+1;
}
int GMTDateTime::getYear() //return actual year
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_year+1900;
}
int GMTDateTime::getHour()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_hour;
}
int GMTDateTime::getMinute()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_min;
}
int GMTDateTime::getSecond()
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_sec;
}
int GMTDateTime::getWeekDay() //return sun to sat in form of 0 to 6
{
if(!(this->isValid))return -1;
return this->gmtDateTime.tm_wday;
}
string GMTDateTime::stringify()
{
if(!this->isValid)return string("");
char buffer[80];
strftime(buffer,80,"%a,%d %b %Y %H:%M:%S GMT",&(this->gmtDateTime));
return string(buffer);
}
void GMTDateTime::addSeconds(int seconds)
{
this->gmtDateTime.tm_sec+=seconds;
mktime(&(this->gmtDateTime));
}
void GMTDateTime::addMinutes(int minutes)
{
this->gmtDateTime.tm_min+=minutes;
mktime(&(this->gmtDateTime));
}
void GMTDateTime::addHours(int hours)
{
this->gmtDateTime.tm_hour+=hours;
mktime(&(this->gmtDateTime));
}
void GMTDateTime::addDays(int days)
{
this->gmtDateTime.tm_mday+=days;
mktime(&(this->gmtDateTime));
}
void GMTDateTime::addMonths(int months)
{
this->gmtDateTime.tm_mon+=months;
mktime(&(this->gmtDateTime));
}
void GMTDateTime::addYears(int years)
{
this->gmtDateTime.tm_year+=years;
mktime(&(this->gmtDateTime));
}
ostream & operator<<(ostream &out, GMTDateTime &gmtDateTime)
{
out<<gmtDateTime.stringify();
return out;
}
