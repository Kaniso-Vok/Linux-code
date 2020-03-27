#pragma once
#include "strTools.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

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
<<<<<<< HEAD
  //试题信息
=======

>>>>>>> b41a6ea875327919859ea6f59615a6d3ff676e3d
  private:
    unordered_map<string,Question> mod1_map;
};


















