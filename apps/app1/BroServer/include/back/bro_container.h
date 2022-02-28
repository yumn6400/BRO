#ifndef __BRO_CONTAINER__
#define __BRO_CONTAINER__ 9999
#include<bro.h>
class Container
{
private:
typedef struct _bag
{
void *ptr;
int size;
}Bag;
map<string,Bag> dataSet;
public:
template<class T>
void set(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason);
template<class T>
void get(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason);
template<class T>
void remove(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason);
bool contains(string keyName);
};


template<class T>
void Container::set(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto iterator=dataSet.find(keyName);
if(iterator!=dataSet.end())
{
if(reason)*reason={__KEY_EXISTS__};
if(success) *success=false;
return;
}
void *ptr;
ptr=malloc(sizeof(T));
if(ptr==NULL)
{
if(reason) *reason={__OUT_OF_MEMORY__};
if(success) *success=false;
return;
}
memcpy(ptr,&value,sizeof(T));
dataSet.insert(pair<string,Bag>(keyName,{ptr,sizeof(T)}));
}
template<class T>
void Container::get(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto dataSetIterator=dataSet.find(keyName);
if(dataSetIterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success)*success=false;
return;
}
Bag bag;
bag=dataSetIterator->second;
if(bag.size!=sizeof(*value))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success)*success=false;
return;
}
memcpy(value,bag.ptr,sizeof(*value));
if(success)*success=true;
}
template<class T>
void Container::remove(string keyName,T value,bool *success,__container_operation_failure_reason__ *reason)
{
auto dataSetIterator=dataSet.find(keyName);
if(dataSetIterator==dataSet.end())
{
if(reason) *reason={__KEY_DOES_NOT_EXIST__};
if(success)*success=false;
return;
}
Bag bag;
bag=dataSetIterator->second;
if(bag.size!=sizeof(*value))
{
if(reason) *reason={__VALUE_SIZE_MISMATCH__};
if(success)*success=false;
return;
}
memcpy(value,bag.ptr,sizeof(*value));
free(bag.ptr);//To release memory allocated by the Bro server programmer
if(success)*success=true;
}
bool Container::contains(string keyName)
{
auto iterator=this->dataSet.find(keyName);
return iterator!=this->dataSet.end();
}

#endif