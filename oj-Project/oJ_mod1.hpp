#pragma once
#include "strTools.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "oJ_log.hpp"

using namespace std;

struct Question{
  string _id;
  string _name;
  string _path;
  string _level;
}QUEST;

class oJ_mod1{
  public:
    oJ_mod1(){
      LoadQuestions("./config_oj.cfg");
    };
    bool GetAllQuestions(vector<Question>* ques){
      for(const auto& e:mod1_map){
        ques->push_back(e.second);
      }
      //根据id排序
      sort(ques->begin(),ques->end(),[](const Question& a,const Question& b){
          return atoi(a._id.c_str()) < atoi(b._id.c_str());
          });
      return true;
    };

//单个题目
bool GetSingleQuestion(const string& id ,string *desc,string *predfe,Question* ques){
  //1.根据id查找对应题目信息，重点是题目在哪里加载
  auto iter = mod1_map.find(id);
  if(iter == mod1_map.end()){
    LOG(ERROR,"QUESTION NOT FOUND id is") << id << endl;
    return false;
  }
  *ques=iter->second;
  //文件路径iter->second._path 
  //加载单个题目信息
  //从具体的题目文件中获取描述和预定义的代码
  int ret = FileOper::LoadDateFromFile(DescPath(iter->second._path),desc);
  if(ret==-1){
    LOG(ERROR,"READ DESC FAILED")<<endl;
    return false;
  }

  ret = FileOper::LoadDateFromFile(PredfePath(iter->second._path),predfe);
  if(ret==-1){
    LOG(ERROR,"READ PREDFE FAILED")<<endl;
    return false;
  }
  return true;
}

//拼接代码
bool SplicingCode(string user_code,const string& ques_id,string* fin_code){
  //1.找题目是否存在
  auto iter = mod1_map.find(ques_id);
  if(iter ==mod1_map.end()){
    LOG(ERROR,"can't find question id is :") <<ques_id<<endl;
    return false;
  }
  string tail_code;
  int ret = FileOper::LoadDateFromFile(TailPath(iter->second._path),&tail_code);
  if(ret < 0){
    LOG(ERROR,"Open tail.cpp failed");
    return false;
  }
  *fin_code = user_code + tail_code;
  return true;
};

  private:
    bool LoadQuestions(const string& configfile_path){
      ifstream file(configfile_path.c_str());
      if(!file.is_open()){
        return false;
      }
      string line;
      //试题信息存在文件中，每一个题目中各个信息以/t分割
      //编写工具 StrTools 用来切割字符串
      while(getline(file,line)){
        vector<string> vec;
        StrTools::Split(line," ",&vec);
        if(vec.size()!=4){
          continue;
        }
        //存入map
        Question ques;
        ques._id=vec[0];
        ques._name=vec[1];
        ques._path=vec[2];
        ques._level=vec[3];
        mod1_map[ques._id]=ques;
      }
      file.close();
      return true;
}
string DescPath(const string desc_path){
  return desc_path+"/desc.txt";
}
  
string PredfePath(const string desc_path){
  return desc_path+"/predfe.cpp";
}
string TailPath(const string desc_path){
  return desc_path+"/tail.cpp";
}

  //试题信息
  private:
   unordered_map<string,Question> mod1_map;
};


















