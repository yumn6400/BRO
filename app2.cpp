#include"bro.cpp"
typedef struct __student_
{
int rollNumber;
char name[21];
char gender;
}Student;
int main()
{
try
{
Bro bro;
bro.setStaticResourcesFolder("C:/bro/apps/app1/static");
bro.get("/",[](Request &request,Response &response){
response.setContentType("text/html");
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<meta charset='utf-8'>
</head>
<body>
<table border='1'>
<thead>
<tr>
<th>S.no</th><th>RollNumber</th><th>Name</th><th>Gender</th><th>Edit</th><th>Delete</th>
</tr>
</thead>
<tbody>
)"""";
FILE *file=fopen("student.dat","rb");
int sno=0;
if(file!=NULL)
{
Student stud;
char str[11];
while(1)
{
fread(&stud,sizeof(Student),1,file);
if(feof(file))break;
sno++;
response<<"<tr>";
itoa(sno,str,10);
response<<"<td>"<<str<<"</td>";
itoa(stud.rollNumber,str,10);
response<<"<td>"<<str<<"</td>";
response<<"<td>"<<stud.name<<"</td>";
if(stud.gender=='M')
{
response<<"<td><img src='Male.png'></td>";
}
else
{
response<<"<td><img src='Female.png'></td>";
}
response<<"<td><a href='/editStudent?rollNumber="<<str<<"'>Edit</a></td>";
response<<"<td><a href='/deleteStudent?rollNumber="<<str<<"'>Delete</a></td>";
response<<"</tr>";
}
fclose(file);
}
if(sno==0)
{
response<<"<tr><td colspan='6' align='center'>No Students</td></tr>";
}
response<<R""""(
</tbody>
</table>
<a href='/studentAddForm.html'>AddStudent</a>
</body>
</html>
)"""";
});
bro.get("/addStudent",[](Request &request,Response &response){
response.setContentType("text/html");
string rollNumber=request["rollNumber"];
string name=request["name"];
string gender=request["gender"];
Student stud;
stud.rollNumber=atoi(rollNumber.c_str());
strcpy(stud.name,name.c_str());
stud.gender=gender[0];
FILE *file=fopen("student.dat","ab");
fwrite(&stud,sizeof(Student),1,file);
fclose(file);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<meta charset='utf-8'>
</head>
<body>
<h1>Student(add module)</h1>
<br/>
<form action='/'>
<br/>
<h3>Student Added</h3>
<button type='submit'>OK</button>
</form>
</body>
</html>
)"""";

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