#include "httplib.h"
#include "oJ_mod1.hpp"
#include <vector>

int main() {
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
        ojmod1.GetAllQuestions();
        resp.set_content("","text/html");
        }));


  vok.listen("192.168.23.131",19999);
  return 0;
}
