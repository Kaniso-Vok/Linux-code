#include "udpsvr.hpp"
using namespace std;

int main(int argc,char* argv[]){
  if(argc!=3){
    cout<<argc<<endl;
    cout<<"./svr [ip] [port]"<<endl;
    return 0;
  }

  string ip=argv[1];
  uint16_t port=atoi(argv[2]);

  udpsvr we;
  if(!we._creat()){
    cout<<"socket failed"<<endl;
    return 0;
  }
  if(!we._bind(ip,port)){
    cout<<"bind failed"<<endl;
    return 0;
  }

  while(1){
    string buff;
    struct sockaddr_in addr;
    we._recv(buff,&addr);
    cout<<"client : ["<<buff.c_str()<<']'<<endl;
    cout<<"sever  : ";
    fflush(stdout);
    cin>>buff;
    we._send(buff,&addr);
  }
  if(we._close() < 0){
    cout<<"close failed"<<endl;
  }
  return 0;
}
