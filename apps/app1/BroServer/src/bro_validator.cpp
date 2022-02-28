#include<bro_validator.h>
Validator::Validator()
{
}
bool Validator::isValidMIMEType(string &mimeType)
{
//right now do nothing 
return true;
}
bool Validator::isValidPath(string &path)
{
return FileSystemUtility::directoryExists(path.c_str());
}
bool Validator::isValidURLFormat(string &url)
{
//right now do nothing 
return true;
}