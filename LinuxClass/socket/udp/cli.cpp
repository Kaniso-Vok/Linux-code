#include "udpsvr.hpp"
using namespace std;

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"./cli [ip] [port]"<<endl;
        return 0;
    }
    string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    udpsvr us;
    if(!us._creat()){
        cout<<"socket failed"<<endl;
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip.c_str());

    while(1){
    string buff;
    getline(cin,buff);
    cout<<"client : ["<<buff.c_str()<<']'<<endl;
    us._send(buff,&addr);
    us._recv(buff,&addr);
    cout<<"sever  : ["<<buff.c_str()<<']'<<endl;
    }
    if(us._close() < 0){
        cout<<"close failed"<<endl;
    }
    return 0;
}
