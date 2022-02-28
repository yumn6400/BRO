#ifndef __BRO_VALIDATOR__
#define __BRO_VALIDATOR__ 9999
#include<bro.h>
#include<bro_file_system_utility.h>
class Validator
{
private:
Validator();
public:
static bool isValidMIMEType(string &mimeType);
static bool isValidPath(string &path);
static bool isValidURLFormat(string &url);
};
#endif