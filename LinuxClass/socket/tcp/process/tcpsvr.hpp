#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;

class tcpsvr{
    public:
        tcpsvr(){sock=-1;}
        ~tcpsvr(){}
        bool _creat(){
            sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_TCP);
            if(sock < 0){
                perror("socket");
                return 0;
            }
        }
        bool _bind(string& ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            addr.sin_addr.s_addr=inet_addr(ip.c_str());
            int ret=bind(sock,(struct sockaddr*)&addr,sizeof(addr));
            if(ret<0){
                perror("bind");
                return 0;
            }
        }
        bool _listen(){}
        bool accept(){}
        bool connect(){}
        bool _send(){}
        bool _recv(){}
        bool _close(){}
    private:
        int sock;
};
