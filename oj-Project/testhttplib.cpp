#include <stdio.h>
#include "httplib.h"
using namespace httplib;

int main(){
  ///////1.搭建http服务器
  Server ser;
  ser.Get("/",[](const Request& req,Response& resp){
      (void)req;
      resp.set_content("<html>kaniso_vok</html>","100"); 
      });
  ser.listen("192.168.23.129",19000);
  return 0;
}
