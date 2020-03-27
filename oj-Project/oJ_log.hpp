#pragma once
#include <iostream>
#include <cstdio>
#include <sys/time.h>
#include <string>
#include <string.h>
using namespace std;
//log服务也是在控制台输出
//输出格式：
//[时间 日志等级 文件:行号]  具体的日志信息

class LogTime{
  public:
    //获取时间戳
    static int64_t GetTimeStamp(){
      struct timeval tval;
      gettimeofday(&tval,NULL);
      return tval.tv_sec;
    }
    //获取格式为年-月-日 时：分：秒 的时间
    static void GetTimeStamp(string* timestamp){
      time_t systime;
      time(&systime);
      tm* st=localtime(&systime);
      char buf[30]={'\0'};
      snprintf(buf,sizeof(buf)-1,"%04d-%02d-%02d %02d:%02d:%02d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
      timestamp->assign(buf,strlen(buf));
    }
};

//日志等级：INFO WARNING ERROR FATAL DEBUG

const char* level[]={
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
    "DEBUG"
};

enum LogLevel{
    INFO=0, 
    WARNING,
    ERROR,
    FATAL,
    DEBUG
};

inline void Log(LogLevel lev,const char* file,int line,const string& logmsg){
    string level_info=level[lev];
    string timestamp;
    LogTime::GetTimeStamp(&timestamp);
//[时间 日志等级 文件:行号]  具体的日志信息
    cout<<"["<<timestamp<<" "<<level_info<<" "<<file<<" "<<":"<<line<<"]"<<logmsg<<endl;
}
#define LOG(lev,msg) Log(lev,__FILE__,__LINE__,msg); 

