#ifndef __BRO_GMT_DATE_TIME
#define __BRO_GMT_DATE_TIME 9999
#include<bro_global.h>
class GMTDateTime
{
private:
struct tm gmtDateTime;
bool isValid;
public:
GMTDateTime(); //to pick sys date time
GMTDateTime(int day,int month,int year,int hour,int minute,int second);
int getDay();
int getMonth(); //return in form of 1 to 12
int getYear(); //return actual year
int getHour();
int getMinute();
int getSecond();
int getWeekDay(); //return sun to sat in form of 0 to 6
string stringify();
void addSeconds(int seconds);
void addMinutes(int minutes);
void addHours(int hours);
void addDays(int days);
void addMonths(int months);
void addYears(int years);
};
ostream & operator<<(ostream &out, GMTDateTime &gmtDateTime);
#endif