#pragma once
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include "oJ_log.hpp"
using namespace std;

//实现一个切割字符串的工具函数
class StrTools
{
  public:
    static void Split(const string& input, const string& split_char, vector<string>* output){
         boost::split(*output, input, boost::is_any_of(split_char), boost::token_compress_off);
  }
};
 
//文件操作
class FileOper{
  public:
    static int LoadDateFromFile(string& filename,string& content){
      ifstream file(filename.c_str());
      if(!file.is_open()){
        LOG(ERROR,"OPEN FILE FAILED filename:")<<filename<<endl;
        return -1;
      }
      string line;
      while(getline(file,line)){
        content+=line+"\n";
      }
      file.close();
      return 0;

    }
};
