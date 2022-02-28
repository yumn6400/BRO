#ifndef __BRO_UTILITIES__
#define __BRO_UTILITIES__ 9999
#include<bro.h>
class BroUtilities
{
private:
BroUtilities();
public:
static bool isHexChar(int w);
static void decode(char *encodedString,char *decodedString);
static void loadMIMETypes(map<string,string> &mimeTypes);
};
#endif