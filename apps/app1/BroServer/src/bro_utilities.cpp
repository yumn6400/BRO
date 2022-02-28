#include<bro_utilities.h>
BroUtilities::BroUtilities()
{
}
bool BroUtilities::isHexChar(int w)
{
if(w>=48&&w<=57)return true;
if(w>='a'&&w<='f')return true;
if(w>='A'&&w<='F')return true;
return false;
}
void BroUtilities::decode(char *encodedString,char *decodedString)
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
void BroUtilities::loadMIMETypes(map<string,string> &mimeTypes)
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