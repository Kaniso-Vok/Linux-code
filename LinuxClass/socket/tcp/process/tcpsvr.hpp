#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
using namespace std;

class tcpsvr{
    public:
        tcpsvr(){sock=-1;}
        
        ~tcpsvr(){}
        
        bool _creat(){
            sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if(sock < 0){
                perror("socket");
                return false;
            }
            return true;
        }

        bool _bind(string& ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            addr.sin_addr.s_addr=inet_addr(ip.c_str());
            int ret=bind(sock,(struct sockaddr*)&addr,sizeof(addr));
            if(ret < 0){
                perror("bind");
                return false;
            }
            return true;
        }

        bool _listen(int backlog){
            int ret=listen(sock,backlog);
            if(ret < 0){
                perror("listen");
                return false;
            }
            return true;
        }

        bool _accept(tcpsvr& we,struct sockaddr_in* addr = NULL){
            struct sockaddr_in outaddr;
            socklen_t leng = sizeof(struct sockaddr_in);
            int retsock = accept(sock,(struct sockaddr*)&outaddr,&leng);
            if(retsock < 0){
                perror("accept");
                return false;
            }
            we.sock=this->sock;
            if(addr!=NULL){
                memcpy(addr,&outaddr,leng);
            }
            return true;
        }

        bool _connect(string& ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            addr.sin_addr.s_addr=inet_addr(ip.c_str());

            int ret=connect(sock,(struct sockaddr*)&addr,sizeof(addr));
            if(ret < 0){
                perror("connect");
                return false;
            }
            return true;
        }
        
        bool _send(string& buff){
            int send_size=send(sock,buff.c_str(),buff.length(),0);
            if(send_size < 0){
                perror("send");
                return false;
            }
            return true;
        }
        
        bool _recv(string& buff){
            char tmp[1024]={0};
            int recv_size=recv(sock,tmp,sizeof(tmp)-1,0);
            if(recv_size < 0){
                perror("recv");
                return false;
            }
            else if(recv_size==0){
                perror("connect failed");
                return false;
            }
            
            buff.assign(tmp,recv_size);
            return true;
        }
        
        bool _close(){
            if(close(sock) < 0){
                perror("close failed");
                return false;
            }
            return true;
        }
    
    private:
        int sock;
};
