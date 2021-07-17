//Amit : the webserver developer
//Bobby : the webside developer
#include<iostream>
using namespace std;
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
public:
void setContentType(string contentType)
{

}
Response & operator<<(string content)
{
return *this;
}
};
class Request
{

};
class Bro
{
public:
void setStaticResourcesFolder(string staticResourcesFolder)
{
//do nothing right now
}
void get(string url,void(*callBack)(Request &,Response &))
{
//do nothing right now
}
void listen(int portNumber,void(*callBack)(Error &))
{
//do nothing right now
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