#include<bro_error.h>
Error::Error(string error)
{
this->error=error;
}
bool Error::hasError()
{
return this->error.length()>0;
}
string Error::getError()
{
return this->error;
}