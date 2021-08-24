#include"bro.cpp"
int main()
{
typedef struct __student__
{
int rollNumber;
char name[21];
char gender;
}Student;
try
{
Bro bro;
bro.setStaticResourcesFolder("c:/bro/gitHub/bro/apps/app1/static");
bro.get("/",[](Request &request,Response &response){
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking Machines</title>
<script>
function confirmStudentDeletion(rollNumber,name)
{
var x=confirm("Delete "+name+" ?");
if(x)
{
var dsf=document.getElementById("deleteStudentForm");
var rn=document.getElementById("rollNumber");
rn.value=rollNumber;
dsf.submit();
}
else
{
alert(name+" Not deleted");
}
}
</script>
</head>
<body>
<h1>List of Student</h1>
<table border='1'>
<thead>
<tr>
<th>S.No.</th><th>Roll Number</th><th>Name</th><th>Gender</th><th>Edit</th><th>Delete</th>
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
fread(&stud,sizeof(stud),1,file);
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
response<<"<td><a href='javascript:confirmStudentDeletion("<<str<<",\""<<stud.name<<"\")'>Delete</a></td>";
response<<"</tr>";
}
fclose(file);
}
if(sno==0)
{
response<<"<tr><td colspan='6' align='center'>No Students </td></tr>";
}
response<<R""""(
</tbody>
</table>
<br/>
<a href='StudentAddForm.html'>Add Student</a>
<form action='deleteStudent' id='deleteStudentForm'>
<input type='hidden' id='rollNumber' name='rollNumber'>
</form>
</body>
</html>
)"""";
});



bro.get("/addStudent",[](Request &request,Response &response)void{
string rollNumber=request["rollNumber"];
string name=request["name"];
string gender=request["gender"];
//We should check if roll number supplied is already exists
int vRollNumber=atoi(rollNumber.c_str());
FILE *file;
file=fopen("student.dat","rb");
if(file!=NULL)
{
Student studtmp;
bool found=false;
while(1)
{
fread(&studtmp,sizeof(Student),1,file);
if(feof(file))break;
if(vRollNumber==studtmp.rollNumber)
{
found=true;
break;
}
}
fclose(file);
if(found)
{
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking Machines</title>
<script>
function validateForm(frm)
{
var rollNumber=frm.rollNumber.value.trim();
if(rollNumber.length==0)
{
alert('Roll number required');
frm.rollNumber.focus();
return false;
}
rollNumber=parseInt(rollNumber);
if(isNaN(rollNumber))
{
alert('Roll number should be of integer type');
frm.rollNumber.value='';
frm.rollNumber.focus();
return false;
}
if(rollNumber<=0)
{
alert('Roll number should be greater than zero');
frm.rollNumber.value='';
frm.rollNumber.focus();
return false;
}
var name=frm.name.value.trim();
if(name.length==0)
{
alert('Name required');
frm.name.focus();
return false;
}
var v='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .';
var i=0;
while(i<name.length)
{
if(v.indexOf(name.charAt(i))==-1)
{
alert('Invalid characters in name');
frm.name.focus();
return false;
}
i++;
}
return true;
}
</script>
</head>
<body>
<h1>Student (Add module)</h1>
)"""";
char line[201];
sprintf(line,"<div style='color:red'>The roll number (%d) has been alloted to somebody else</div>",vRollNumber);
response<<line;
response<<R""""(
<form action='addStudent' onsubmit='return validateForm(this)'>
RollNumber
)"""";
sprintf(line,"<input type='text' id='rollNumber' name='rollNumber' value='%d'>",vRollNumber);
response<<line;
response<<"<br/>";
response<<"Name";
sprintf(line,"<input type='text' id='name' name='name' maxlength='20' value='%s'>",name.c_str());
response<<line;
response<<"<br/>";
response<<"Gender &nbsp;&nbsp;&nbsp;";
if(gender[0]=='M')
{
response<<R""""(
Male
<input type='radio' id='male' name='gender' value='M' checked>
Female
<input type='radio' id='female' name='gender' value='F'>
)"""";
}
else
{
response<<R""""(
Male
<input type='radio' id='male' name='gender' value='M'>
Female
<input type='radio' id='female' name='gender' value='F' checked>
)"""";
}
response<<R""""(
<br/>
<br/>
<button type='submit'>Add</button>
</form>
<br/><br/>
<a href='/'>Home</a>
</body>
</html>
)"""";
return;
}//if found ends here
}//if file is not NULL ends here
Student stud;
stud.rollNumber=atoi(rollNumber.c_str());
strcpy(stud.name,name.c_str());
stud.gender=gender[0];
file=fopen("student.dat","ab");
fwrite(&stud,sizeof(Student),1,file);
fclose(file);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking Machines</title>
</head>
<body>
<h1> Student(Add Module)</h1>
<br/><br/>
<h3>Student Added</h3>
<br/>
<form action='/'>
<button type='submit'>OK</button>
</form>
<br/>
<a href='/'>Home</a>
</body>
</html>
)"""";
});
bro.get("/editStudent",[](Request &request,Response &response){
string rollNumber=request["rollNumber"];
int vRollNumber=atoi(rollNumber.c_str());
FILE *file=fopen("student.dat","rb");
Student stud;
while(1)
{
fread(&stud,sizeof(Student),1,file);
if(feof(file))break;
if(stud.rollNumber==vRollNumber)break;
}
fclose(file);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking Machines</title>
<script>
function validateForm(frm)
{
var name=frm.name.value.trim();
if(name.length==0)
{
alert('Name required');
frm.name.focus();
return false;
}
var v='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .';
var i=0;
while(i<name.length)
{
if(v.indexOf(name.charAt(i))==-1)
{
alert('Invalid characters in name');
frm.name.focus();
return false;
}
i++;
}
return true;
}
</script>
</head>
<body>
<h1>Student (Edit module)</h1>
<form action='updateStudent' onsubmit='return validateForm(this)'>
RollNumber
)"""";
char str[11];
itoa(stud.rollNumber,str,10);
response<<str;
char line[101];
sprintf(line,"<input type='hidden' id='rollNumber' name='rollNumber' value='%d'>",vRollNumber);
response<<line;
response<<R""""(
<br/>
Name 
)"""";
sprintf(line,"<input type='text' id='name' name='name' maxlength='20' value='%s'>",stud.name);
response<<line;
response<<R""""(
<br/>
Gender &nbsp;&nbsp;&nbsp;
)"""";
if(stud.gender=='M')
{
response<<R""""(
Male
<input type='radio' id='male' name='gender' value='M' checked>
Female
<input type='radio' id='female' name='gender' value='F'>
)"""";
}
else
{
response<<R""""(
Male
<input type='radio' id='male' name='gender' value='M'>
Female
<input type='radio' id='female' name='gender' value='F' checked>
)"""";
}
response<<R""""(
<br/>
<br/>
<button type='submit'>Update</button>
</form>
<br/><br/>
<a href='/'>Home</a>
</body>
</html>
)"""";
});
bro.get("/updateStudent",[](Request &request,Response &response){
string rollNumber=request["rollNumber"];
string name=request["name"];
string gender=request["gender"];
Student stud;
stud.rollNumber=atoi(rollNumber.c_str());
strcpy(stud.name,name.c_str());
stud.gender=gender[0];
FILE *f1=fopen("student.dat","rb");
FILE *f2=fopen("tmp.dat","wb");
Student tmpstud;
while(1)
{
fread(&tmpstud,sizeof(Student),1,f1);
if(feof(f1))break;
if(tmpstud.rollNumber==stud.rollNumber)
{
fwrite(&stud,sizeof(Student),1,f2);
}
else
{
fwrite(&tmpstud,sizeof(Student),1,f2);
}
}
fclose(f1);
fclose(f2);
f1=fopen("student.dat","wb");
f2=fopen("tmp.dat","rb");
while(1)
{
fread(&tmpstud,sizeof(Student),1,f2);
if(feof(f2))break;
fwrite(&tmpstud,sizeof(Student),1,f1);
}
fclose(f1);
fclose(f2);
f2=fopen("tmp.dat","wb");
fclose(f2);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking machines</title>
</head>
<body>
<h1>Student (Edit Module)</h1>
<br/><br/>
<h3> Student Updated<h3>
<form action='/'>
<button type='submit'>OK</button>
</form>
<br/>
<a href='/'>Home <a>
</body>
)"""";
});


bro.get("/confirmStudentDeletion",[](Request &request,Response &response){
string rollNumber=request["rollNumber"];
int vRollNumber=atoi(rollNumber.c_str());
FILE *file=fopen("student.dat","rb");
Student stud;
while(1)
{
fread(&stud,sizeof(Student),1,file);
if(feof(file))break;
if(stud.rollNumber==vRollNumber)break;
}
fclose(file);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking Machines</title>
</head>
<body>
<h1>Student (Delete module)</h1>
<form action='deleteStudent'>
RollNumber  :
)"""";
char str[11];
itoa(stud.rollNumber,str,10);
response<<str;
char line[101];
sprintf(line,"<input type='hidden' id='rollNumber' name='rollNumber' value='%d'>",vRollNumber);
response<<line;
response<<R""""(
<br/>
Name  :
)"""";
response<<stud.name;
response<<R""""(
<br/>
Gender  :
)"""";
if(stud.gender=='M')
{
response<<"Male";
}
else
{
response<<"Female";
}
response<<R""""(
<br/>
<br/>
<button type='submit'>Delete</button>
</form>
<br/><br/>
<a href='/'>Home</a>
</body>
</html>
)"""";
});

bro.get("/deleteStudent",[](Request &request,Response &response){
string rollNumber=request["rollNumber"];
int vRollNumber=atoi(rollNumber.c_str());
FILE *f1=fopen("student.dat","rb");
FILE *f2=fopen("tmp.dat","wb");
Student stud;
while(1)
{
fread(&stud,sizeof(Student),1,f1);
if(feof(f1))break;
if(stud.rollNumber!=vRollNumber)
{
fwrite(&stud,sizeof(Student),1,f2);
}
}
fclose(f1);
fclose(f2);
f1=fopen("student.dat","wb");
f2=fopen("tmp.dat","rb");
while(1)
{
fread(&stud,sizeof(Student),1,f2);
if(feof(f2))break;
fwrite(&stud,sizeof(Student),1,f1);
}
fclose(f1);
fclose(f2);
f2=fopen("tmp.dat","wb");
fclose(f2);
response<<R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Thinking machines</title>
</head>
<body>
<h1>Student (Delete Module)</h1>
<br/><br/>
<h3> Student Deleted<h3>
<form action='/'>
<button type='submit'>OK</button>
</form>
<br/>
<a href='/'>Home <a>
</body>
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