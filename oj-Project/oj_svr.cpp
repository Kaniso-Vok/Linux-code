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
    //(\d)只能匹配一个数字（0-9），要匹配多位数字需要改为（\d+),加括号是因为 matches[] 的分组应用
    vok.Get(R"(/question/(\d+))",[&ojmod1](const Request& req, Response& resp){
        //1.去试题模块（map）查找对应题目的信息（序号名称地址难度）

        //      cout<<req.path.c_str()<<endl;
        //      cout<<req.matches[0]<<endl<<req.matches[1]<<endl;
        
        //2.去对应题目的路径下家在单个题目的描述信息
        string desc,predfe;
        Question ques;
        ojmod1.GetSingleQuestion(req.matches[1].str(),&desc,&predfe,&ques);

        //3.组织信息并返回
        string html;
        oJview::ExpandSingleQuestion(ques,desc,predfe,&html);
        resp.set_content(html,"text/html;charset=UTF-8");
        });
    LOG(INFO,"Listen to 192.168.23.131:19999")<<endl;
    LOG(INFO,"Server Ready")<<endl;
    //Listen 会进行阻塞
    vok.listen("192.168.23.132",19999);
  return 0;
}
