#ifndef __BRO_ERROR__
#define __BRO_ERROR__
#include<bro.h>
class Error
{
private:
string error;
public:
Error(string error);
bool hasError();
string getError();
};
#endif