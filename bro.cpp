//Amit : the webserver developer
//Bobby : the webside developer
#include<iostream>
#include<forward_list>
#include<map>
using namespace std;
class Validator
{
private:
Validator()
{

}
public:
static bool isValidPath(string &path)
{
return true;
}
static bool isValidURLFormat(string &url)
{
return true;
}
static bool isValidMIMEType(string &mimeType)
{
return true;
}

};
class Error
{
public:
bool hasError()
{
return false;
}
string getError()
{
return "";
}
};
class Response
{
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
void setContentType(string contentType)
{
if(Validator::isValidMIMEType(contentType))
{
this->contentType=contentType;
}
}
Response & operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
};
class Request
{

};
class Bro
{
private :
string staticResourcesFolder;
map<string,void(*)(Request &,Response &)> urlMappings;
public:
void setStaticResourcesFolder(string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder;
}
}
void get(string url,void(*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
urlMappings.insert(pair<string,void(*)(Request &,Response &)>(url,callBack));
}
}
void listen(int portNumber,void(*callBack)(Error &))
{

}
};
int main()
{
Bro bro;
bro.setStaticResourcesFolder("whatever");
bro.get("/",[](Request &request,Response &response)void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
</head>
<body>
<h1>Welcome</h1>
<h3>Administration</h3>
<a href='getCustomers'>Customer List</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/getCustomers",[](Request &request,Response &response)void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
</head>
<body>
<h1>Customer List</h1>
<ui>
<li>Suresh</li>
<li>Ramesh</li>
<li>Ganesh</li>
</ui>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.listen(6060,[](Error &error)void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro http web server is ready to accept request on port 6060"<<endl;
});
return 0;
}