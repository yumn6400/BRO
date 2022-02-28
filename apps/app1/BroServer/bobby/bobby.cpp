#include<bro.h>
//Bobby[the web application developer]
class Bulb:public Stringifyable
{
private:
int wattage;
int price;
public:
void setWattage(int wattage)
{
this->wattage=wattage;
}
int getWattage()
{
return this->wattage;
}
void setPrice(int price)
{
this->price=price;
}
int getPrice()
{
return this->price;
}
string stringify()
{
return string("Wattage is: ")+to_string(this->wattage)+string(", Price is: ")+to_string(this->price);
}
};
int main()
{
try
{
Bro bro;
bro.setStaticResourcesFolder("whatever");
bro.get("/slogan",[](Request &request,Response &response) void{
string slogan;
ifstream iFile("data/sofd.data");
string line;
while(true)
{
if(!getline(iFile,line))break;
if(slogan.length()>0)slogan+=string("<br>");
slogan+=line;
}
iFile.close();
cout<<"Slogon of the day: "<<slogan<<endl;
request.setCHTMLVariable("sloganOfTheDay",slogan);
request.setCHTMLVariable("abcd","Hello");
Bulb bulb;
bulb.setWattage(60);
bulb.setPrice(100);
request.setCHTMLVariable("Bulb",&bulb);
request.setCHTMLVariable("aa",100);
request.setCHTMLVariable("bb",'a');
request.setCHTMLVariable("cc",100.88);
_forward_(request,string("/wordsOfWisdom.chtml"));
});


bro.addStartupService(3,[](){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 3"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.addStartupService(1,[](){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 1"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.addStartupService(2,[](ApplicationLevelContainer &applicationLevelContainer){
cout<<"-------------------------------------------------"<<endl;
cout<<"Some cool function that get called on startup"<<endl;
cout<<"The priority number setup for this function is 2"<<endl;
cout<<"-------------------------------------------------"<<endl;
});
bro.get("/",[](Request &request,Response &response) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Bro test cases</title>
<body>
<h1>Welcome</h1>
<br/><br/>
<a href='/coolBro1'>First Film</a><br/>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.get("/coolBro1",[](Request &request,Response &response) void{
cout<<"/coolBro1 function got called"<<endl;
_forward_(request,string("/coolBro2"));
cout<<"Code after forwarding function"<<endl;
});
bro.get("/coolBro2",[](Request &request,Response &response) void{
cout<<"/coolBro2 function got called"<<endl;
_forward_(request,string("/coolBro3"));
});
bro.get("/coolBro3",[](Request &request,Response &response) void{
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Bro test cases</title>
<body>
<h1>Welcome</h1>
<br/><br/>
<h1> CoolBro3 function got called</h1>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
cout<<request.getCookieValue("RollNumber")<<endl;
cout<<request.getCookieValue("Name")<<endl;
cout<<request.getCookieValue("Gender")<<endl;
list<string> l1;
request.getCookieNames(l1);
list<Cookie> l2;
request.getCookies(l2);
cout<<"***************"<<endl;
for(string s:l1)
{
cout<<s<<endl;
}
cout<<"***************"<<endl;
for(Cookie c:l2)
{
cout<<c.getName()<<","<<c.getValue()<<endl;
}



cout<<"/coolBro3 function got called"<<endl;
});

bro.get("/aRequest",[](Request &request,Response &response)void{
cout<<"Function against /aRequest got called"<<endl;
int x=100;
//100 as a score is generated according to application requirement
request.set("score",x,NULL,NULL);
cout<<"Now forwarding request for /bRequest"<<endl;
_forward_(request,string("/bRequest"));
});
bro.get("/bRequest",[](Request &request,Response &response)void{
cout<<"Function against /bRequest got called"<<endl;
int whatever;
request.get("score",&whatever,NULL,NULL);
cout<<"Fetched data form request object,which was named as score"<<endl;
response.setContentType("text/html");
Cookie cookie("Name","Rahul");
Cookie cookie1("RollNumber","101");
Cookie cookie2("Gender","Male");
response<<cookie<<cookie1<<cookie2;
response<<"<html><head></head><body>";
response<<"<br><a href=\"/coolBro1\">coolBro1</a></body></html>";
});

bro.listen(6060,[](Error &error)void{
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro HTTP server is ready to accept request on port 6060"<<endl;
});
}catch(string exception)
{
cout<<exception<<endl;
}
return 0;
}
