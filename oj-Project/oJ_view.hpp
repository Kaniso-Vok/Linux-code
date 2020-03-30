#pragma once
#include <ctemplate/template.h>
#include "oJ_mod1.hpp"
#include <vector>
#include <string>
using namespace std;

class oJview{
  public:
    //渲染AllQuestionshtml页面
  static void ExpandAllQuestionsHtml(string* html,vector<Question>& ques){
    //1.获取数据字典，将拿到的试题数据按顺序存储到内存
    ctemplate::TemplateDictionary dict("all_questions");

    for(const auto& e:ques){
      //拿到子字典，利用子字典填充
      ctemplate::TemplateDictionary* section_dict=dict.AddSectionDictionary("questions");

      section_dict->SetValue("_id",e._id);
      section_dict->SetValue("_id",e._id);
      section_dict->SetValue("_name",e._name);
      section_dict->SetValue("_level",e._level);
    }

    //2.获取模板类指针，加载预定义的html页面到内存中
    ctemplate::Template* t1=ctemplate::Template::GetTemplate("./template/all_questions.html",ctemplate::DO_NOT_STRIP);
    //3.渲染，拿着模板类指针将数据字典中的数据更新到html页面的内存中
    t1->Expand(html,&dict);
  }
  //渲染单个题目界面（作答界面）
  static void ExpandSingleQuestion(const Question& ques,string& _desc,string& _predfe,string* html){
    ctemplate::TemplateDictionary dict("question");
    dict.SetValue("id",ques._id);
    dict.SetValue("name",ques._name);
    dict.SetValue("level",ques._level);
    dict.SetValue("desc",_desc);
    dict.SetValue("predfe",_predfe);
    
    ctemplate::Template* ret=ctemplate::Template::GetTemplate("./template/question.html",ctemplate::DO_NOT_STRIP);
    ret->Expand(html,&dict);
  }


};
