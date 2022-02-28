#include<bro_template_engine.h>
void TemplateEngine::createVMDFileName(const char *chtmlFileName,char *vmdFileName)
{
char *dotPtr;
const char *ptr;
ptr=chtmlFileName;
while(*ptr!='\0')ptr++;
while(ptr>=chtmlFileName)
{
if(*ptr=='/')
{
chtmlFileName=ptr+1;
break;
}
ptr--;
}
for(;*chtmlFileName!='\0';chtmlFileName++,vmdFileName++)
{
if(*chtmlFileName=='.')dotPtr=vmdFileName;
*vmdFileName=*chtmlFileName;
}
*vmdFileName='\0';
strcpy(dotPtr+1,"vmd");
}

void TemplateEngine::createVMDFile(const char *chtmlFileName,const char *pathToVMDFile)
{
char  *start,*end,*ptr;
unsigned int x;
FILE *f;
int toRead;
char buffer[513];
long fileLength;
f=fopen(chtmlFileName,"rb");
if(f==NULL)
{
//code to send back 404 error
return;
}
FILE *vmdFile;
vmdFile=fopen(pathToVMDFile,"wb");
if(vmdFile==NULL)
{
//code to send back 404 error
fclose(f);
return;
}
fseek(f,0,SEEK_END);
fileLength=ftell(f);
rewind(f);
unsigned int sp,ep;
int ii;
struct vmd vmd_record;
x=0;
while(1)
{
if((fileLength-x)>256)toRead=256;
else toRead=fileLength-x;
fread(buffer,toRead,1,f);
buffer[toRead]='\0';
x=x+toRead;
//processing the content of buffer starts here
start=buffer;
while(1)
{
start=strstr(start,"${");
if(start==NULL)
{
if(x==fileLength)break;
if(buffer[toRead-1]=='$')
{
buffer[0]='$';
if((fileLength-x)>255)toRead=255;
else toRead=fileLength-x;
fread(buffer+1,toRead,1,f);
x=x+toRead;
buffer[1+toRead]='\0';
start=buffer;
continue;
}
else
{
break;
}
}
end=strstr(start+2,"}");
if(end!=NULL)
{
for(ptr=start+1;ptr<end;ptr++)
{
if(*ptr=='$'&&*(ptr+1)=='{')break;
}
if(*ptr=='$')start=ptr;
//write vmd record to the file starts here
for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
{
vmd_record.var_name[ii]=*ptr;
}
vmd_record.var_name[ii]='\0';
sp=(x-toRead)+(start-buffer);
ep=(x-toRead)+(end-buffer);
cout<<"1-->"<<sp<<","<<ep<<endl;
vmd_record.start_position=sp;
vmd_record.end_position=ep;
fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
//write vmd record to the file ends here
start=end+1;

continue;
}
//if } not found,then load the next chunk of 256 bytes and store
//it in buffer from index 256
if(x==fileLength)break;
if((fileLength-x)>256)toRead=256;
else toRead=fileLength-x;
fread(buffer+256,toRead,1,f);
buffer[toRead+256]='\0';
x=x+toRead;
end=strstr(buffer+256,"}");
if(end==NULL)break;
for(ptr=start+1;ptr<end;ptr++)
{
if(*ptr=='$'&&*(ptr+1)=='{')break;
}
if(*ptr=='$')start=ptr;
//write vmd record to the file starts here
for(ptr=start+2,ii=0;ptr<end;ptr++,ii++)
{
vmd_record.var_name[ii]=*ptr;
}
vmd_record.var_name[ii]='\0';
sp=(x-toRead)+(start-buffer)-256;
ep=(x-toRead)+(end-buffer)-256;
cout<<"2-->"<<sp<<","<<ep<<endl;
vmd_record.start_position=sp;
vmd_record.end_position=ep;
fwrite(&vmd_record,sizeof(struct vmd),1,vmdFile);
strcpy(buffer,buffer+256);
end=end-256;
start=end+1;
}//while(1) ends
//processing the contents of the buffer ends here
if(x==fileLength)break;
}
fclose(f);
fclose(vmdFile);
}

void TemplateEngine::processCHTMLFile(const char *chtmlFileName,Request &request,int clientSocketDescriptor)
{
if(!FileSystemUtility::directoryExists("vmd_files"))
{
if(!FileSystemUtility::createDirectory("vmd_files"))
{
//we will implement this part later on
//the code to calculate response size starts here
//the code to calculate response size ends here
}
}
char vmdFileName[257];
createVMDFileName(chtmlFileName,vmdFileName);
string folderName=string("vmd_files");
string pathToVMDFile=folderName+string(PATH_SEPARATOR)+string(vmdFileName);
bool generateVMDFile=false;
if(FileSystemUtility::fileExists(pathToVMDFile.c_str()))
{
if(FileSystemUtility::getLastUpdatedTime(chtmlFileName)>FileSystemUtility::getLastUpdatedTime(pathToVMDFile.c_str()))
{
generateVMDFile=true;
}
}
else
{
generateVMDFile=true;
}
if(generateVMDFile)
{
createVMDFile(chtmlFileName,pathToVMDFile.c_str());
}
//process the chtml file (pick up info from vmd file)
//code to calculate response size starts here
FILE *chtmlFile=fopen(chtmlFileName,"rb");
FILE *vmdFile=fopen(pathToVMDFile.c_str(),"rb");
fseek(chtmlFile,0,SEEK_END);
long fileLength=ftell(chtmlFile);
rewind(chtmlFile);
string data;
long responseSize=fileLength;
struct vmd vmdRecord;
while(1)
{
fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
if(feof(vmdFile))break;
responseSize=(responseSize-(vmdRecord.end_position-vmdRecord.start_position)+1);
data=request.getCHTMLVariable(vmdRecord.var_name);
responseSize=responseSize+data.length();
}
//code to calculate response size ends here
//code to process chtml file starts here
string mimeType;
mimeType=string("text/html");
char header[200];
sprintf(header,"HTTP/1.1 200 OK\r\n Content-Type:%s\r\n Content-Length:%ld\r\n Connection:close\r\n\r\n",mimeType.c_str(),responseSize);
send(clientSocketDescriptor,header,strlen(header),0);
long bytesLeftToRead;
int bytesToRead;
char buffer[4096];
bytesLeftToRead=fileLength;
rewind(vmdFile);
long tmpBytesLeftToRead;
long bytesProcessFromFile=0;
while(1)
{
fread(&vmdRecord,sizeof(struct vmd),1,vmdFile);
if(feof(vmdFile))break;
tmpBytesLeftToRead=vmdRecord.start_position-bytesProcessFromFile;
bytesToRead=4096;
while(tmpBytesLeftToRead>0)
{
if(tmpBytesLeftToRead<bytesToRead)bytesToRead=tmpBytesLeftToRead;
fread(buffer,bytesToRead,1,chtmlFile);
if(feof(chtmlFile))break;//this will not happen
//send(clientSocketDescriptor,buffer,bytesToRead,0);
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesProcessFromFile+=bytesToRead;
tmpBytesLeftToRead-=bytesToRead;
}//inner loop ends
fread(buffer,(vmdRecord.end_position-vmdRecord.start_position)+1,1,chtmlFile);
bytesProcessFromFile+=((vmdRecord.end_position-vmdRecord.start_position)+1);
if(request.containsCHTMLVariable(vmdRecord.var_name))
{
data=request.getCHTMLVariable(vmdRecord.var_name);
send(clientSocketDescriptor,data.c_str(),data.length(),0);
}
}//outer loop ends

//vmd file ends but there may be any content is chtmlFile
//God is ${great}.Ujjain is the city of ${Gods}.I live in Ujjain.
//here vmd file has 2 record after completion is does not send
//I live in Ujjain.

bytesLeftToRead-=bytesProcessFromFile;
bytesToRead=4096;
while(bytesLeftToRead>0)
{
if(bytesLeftToRead<bytesToRead)bytesToRead=bytesLeftToRead;
fread(buffer,bytesToRead,1,chtmlFile);
if(feof(chtmlFile))break; //this will never happen
send(clientSocketDescriptor,buffer,bytesToRead,0);
bytesLeftToRead=bytesLeftToRead-bytesToRead;
}

//code to process chtml file ends here
fclose(chtmlFile);
fclose(vmdFile);
//close(clientSocketDescriptor);
}