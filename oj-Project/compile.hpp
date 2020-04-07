#pragma once
#include <iostream>
#include "json/json.h"
#include "oJ_log.hpp"
#include <string>
#include "strTools.hpp"
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>

enum ErrorNum {
  OK = 0,
  COMPLIE_ERROR,
  RUN_ERROR,
  PRAM_ERROR,
  INTERNAL_ERROR
};

class Compiler{
  public:
    //有可能浏览器对不同题目提交的数据不同
    //类似：code="xxx"
    //      code="xxx"&text="xxx"
    static void ComplieAndRun(Json::Value req,Json::Value* resp){
      //0:编译运行没有错误
      //1：编译错误
      //2：运行错误
      //3：参数错误
      //4：内存错误
      
      
      //1.判空，检查对应字段是否有内容
      if(req["code"].empty()){
        (*resp)["errornum"] = PRAM_ERROR;
        (*resp)["reason"] = "Pram error";
        LOG(ERROR,"Request code is Empty")<<endl;
        return;
      }
      //2.将内容写入文件
      string code = req["code"].asString();
      //文件名称进行约定 tmp_时间戳.cpp  --为多用户服务
      string tmp_filename =WriteTmpFile(code);
      if(tmp_filename == ""){
        (*resp)["errornum"] = INTERNAL_ERROR;
        (*resp)["reason"] = "Create file failed";
        LOG(ERROR,"Write Source Failed");
        return;
      }
      //3.编译  
      if(!Compile(tmp_filename)){
        (*resp)["errornum"] = COMPLIE_ERROR;
        string reason;
        FileOper::LoadDateFromFile(ErrorPath(tmp_filename),&reason);
        (*resp)["reason"] = reason;
        LOG(ERROR,"Complie Error")<<endl;
      }
      //4.运行
      int sig = Run(tmp_filename);
      if(sig != 0){
        (*resp)["errornum"] = RUN_ERROR;
         (*resp)["reason"] = "Program exit by sig" + to_string(sig);
        LOG(ERROR,"Run Error")<<endl;
        return;
      }
      //5.构造响应
      (*resp)["errornum"] = OK; 
      (*resp)["reason"] = "Complie and Run is OK";

      //标准输出
      string stdout_reason;
      FileOper::LoadDateFromFile(StdoutPath(tmp_filename),&stdout_reason);
      (*resp)["stdout"] = stdout_reason;
      //标准错误
      string stderr_reason;
      FileOper::LoadDateFromFile(StderrPath(tmp_filename),&stderr_reason);
      (*resp)["stderr"] = stderr_reason;
      
      //6.清理产生的临时文件
      Clean(tmp_filename);
      

      return ;
    }
  private:
    static string WriteTmpFile(const string& code){
      //1.首先组织文件名称，用来区分源码文件和可执行文件是同一组数据
      string tmp_filename="/tmp_"+to_string(LogTime::GetTimeStamp());
      //写文件
      int ret = FileOper::WriteDateToFile(SrcPath(tmp_filename),code);
      if(ret < 0){
        LOG(ERROR,"Write code to source Failed");
        return "";
      }
      return tmp_filename;
    }
    
    static string SrcPath(const string& filename){
      return "./tmp_files"+filename+".cpp";
    }

    static string ExePath(const string& filename){
      return "./tmp_files"+filename+".executable";
    }
    
    static string ErrorPath(const string& filename){
      return "./tmp_files"+filename+".err";
    }
    
    static string StdoutPath(const string& filename){
      return "./tmp_files"+filename+".stdout";
    }
    
    static string StderrPath(const string& filename){
      return "./tmp_files"+filename+".stderr";
    }
    
    static bool Compile(const string& filename) {
      //1.构造编译命令--g++ src -o [exec] -std=c++11
      const int commadncount = 20;
      char buf[commadncount][50]={{0}};
      char* Command[commadncount]={0};
      for(int i = 0;i < commadncount;i++){
        Command[i] = buf[i];
      } 
      snprintf(Command[0],49,"%s","g++");
      snprintf(Command[1],49,"%s",SrcPath(filename).c_str());
      snprintf(Command[2],49,"%s","-o");
      snprintf(Command[3],49,"%s",ExePath(filename).c_str());
      snprintf(Command[4],49,"%s","-std=c++11");
      snprintf(Command[5],49,"%s","-D");
      snprintf(Command[6],49,"%s","CompileOnline");
      Command[7] = NULL;
      //2.创建子进程
      //2.1 父进程---等待子进程退出
      //2.2子进程---程序替换---》g++编译源码文件
      int pid =fork();
      if(pid < 0){
        LOG(ERROR,"Create child thread Failed") << endl;
        return false;
      } 
      else if(pid == 0){
        //child
        int fd =open(ErrorPath(filename).c_str(),O_CREAT | O_RDWR, 0664);
        if(fd < 0){
          LOG(ERROR,"open Complie ErrorFile failed")<<ErrorPath(filename)<<endl;
          exit(1);
        }
        //重定向
        dup2(fd,2);
        //程序替换
        execvp(Command[0],Command);
        //perror("execvp");
        LOG(ERROR,"execvp failed") << endl;
        exit(0); 
      }
      else{
        //father
        waitpid(pid,NULL,0);
      }
      //3.验证是否生成可执行程序
      struct stat st;
      int ret =stat(ExePath(filename).c_str(),&st);
      if(ret < 0){
        LOG(ERROR,"Compile ERROR : Exe filename is ")<<ExePath(filename)<<endl;
        return false;
      }
      return true;
    }


    static int Run(const string& filename){
      //可执行程序
      //1.创建子进程
      //  父进程 进程等待
      //  子进程 替换编译出来的程序
      int pid =fork();
      if(pid < 0){
        LOG(ERROR,"Exec pragma failed Create child press failed")<<endl;       
        return -1;
      }
      else if(pid == 0){
        //对于子进程执行的限制
        //1.时间限制---alarm
        alarm(1);
        //2.内存大小限制
        struct rlimit rl; 
        rl.rlim_cur = 1024 *30000;
        rl.rlim_max = RLIM_INFINITY; //无限制
        setrlimit(RLIMIT_AS, &rl);

        //child
        //获取：标准输出--》重定向到文件
        int stdout_fd=open(StdoutPath(filename).c_str(),O_CREAT | O_RDWR,0664);
        if(stdout_fd < 0){
          LOG(ERROR,"Open stdout file failed ")<<StdoutPath(filename)<<endl;
          return -1;
        }
        dup2(stdout_fd,1);
        //标准错误--》重定向到文件
        int stderr_fd=open(StderrPath(filename).c_str(),O_CREAT | O_RDWR,0664);
        if(stderr_fd < 0){
          LOG(ERROR,"Open stderr file failed ")<<StderrPath(filename)<<endl;
          return -1;
        }
        dup2(stdout_fd,2);
        execl(ExePath(filename).c_str(),ExePath(filename).c_str(),NULL);
        exit(1);
      }        

      //father
      int Status= -1;
      waitpid(pid,&Status,0);
      //将是否收到信号的信息返回给调用者，如果调用者判断是0则正常运行完毕，否则收到某个信号异常结束
      return Status & 0x7f;//coredump标志位
    };

    static void Clean(string filename){
      unlink(SrcPath(filename).c_str());
      unlink(ExePath(filename).c_str());
      unlink(ErrorPath(filename).c_str());
      unlink(StdoutPath(filename).c_str());
      unlink(StderrPath(filename).c_str());
    }

};
