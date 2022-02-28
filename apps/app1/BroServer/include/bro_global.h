#ifndef __BRO_GLOBAL__
#define __BRO_GLOBAL__ 9999
#include<iostream>
#include<bits/stdc++.h>
#include<forward_list>
#include<map>
#include<stdio.h>
#include<queue>
#include<vector>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<set>
extern const char *PATH_SEPARATOR;
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
using namespace std;
//Amit (The Bro Programmer)
#define _forward_(request,url) \
request.forwardTo(url); \
return;
enum __container_operation_failure_reason__{__KEY_EXISTS__,__KEY_DOES_NOT_EXIST__,__OUT_OF_MEMORY__,__VALUE_SIZE_MISMATCH__};
enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__HEAD__,__OPTIONS__,__CONNECT__,__TRACE__};
#endif