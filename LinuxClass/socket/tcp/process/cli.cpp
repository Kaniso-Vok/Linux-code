#include "tcpsvr.hpp"

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"./cli [ip] [port]"<<endl;
        return 0;
    }
    tcpsvr us;
    string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    if(!us._creat()){
        cout<<"creat failed"<<endl;
        return 0;
    }
    if(!us._connect(ip,port)){
        cout<<"connect failed"<<endl;
        return 0;
    }

    while(1){
        cout<<"client say :";
        fflush(stdout);
        string buff;
        getline(cin,buff);
        us._send(buff);
        us._recv(buff);
        cout<<"sever say :"<<buff<<endl;
    }
    if(!us._close()){
        cout<<"close failed"<<endl;
        return 0;
    }
    return 0;
}
