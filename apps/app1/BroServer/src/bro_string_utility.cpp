#include<bro_string_utility.h>
StringUtility::StringUtility()
{
}
void StringUtility::toLowerCase(char *str)
{
if(str==NULL)return;
for(;*str;str++)if(*str>=65&&*str<=90)*str+=32;
}