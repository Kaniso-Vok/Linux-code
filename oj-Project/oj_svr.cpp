#include "httplib.h"
#include "oJ_mod1.hpp"
#include "oJ_view.hpp"
#include <vector>
#include <stdio.h>
#include "oJ_log.hpp"

int main(){
  using namespace httplib;
  Server vok;
  //1.获取题目信息，从文件中获取
    oJ_mod1 ojmod1;
//  httplib定义:
//  typedef std::function<void (const Request&, Response&)> Handler;                    
//  Server& Get(const char* pattern, Handler handler);                                                    
//  Server& Post(const char* pattern, Handler handler); 
    vok.Get("/all_questions",[&ojmod1](const Request& req, Response& resp){
        vector<Question> ques;
        ojmod1.GetAllQuestions(&ques);
        // char buf[10240]={'\0'};
        //格式:<html>1.二叉树 低级</html>
       // snprintf(buf,sizeof(buf)-1,"<html>%s.%s %s</html>",ques[0]._id.c_str(),ques[0]._name.c_str(),ques[0]._level.c_str());
        //使用字符串承载内容
       // string html;
       // html.assign(buf,strlen(buf));
        //使用模板技术填充html文件
        string html;
        oJview::ExpandAllQuestionsHtml(&html,ques);
       //LOG(INFO,html);
        resp.set_content(html,"text/html;charset=UTF-8");
        });
    //使用正则表达式匹配单个问题的请求  R"(str)"
    //(\d)只能匹配一个数字（0-9），要匹配多位数字需要改为（\d+)
    vok.Get(R"(/question/\d+)",[&ojmod1](const Request& req, Response& resp){
        string html="131";
        resp.set_content(html,"text/html;charset=UTF-8");
        });
    LOG(INFO,"Listen to 192.168.23.131:19999");
    LOG(INFO,"Server Ready");
    //Listen 会进行阻塞
    vok.listen("192.168.23.131",19999);
  return 0;
}
