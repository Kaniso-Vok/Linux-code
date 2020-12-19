#include "tcpsvr.hpp"
#include <sys/wait.h>
#include <pthread.h>

//多线程入口函数
void* threadstart(void* arg){
    pthread_detach(pthread_self()); //分离当前线程防止发生内存泄漏
    tcpsvr* ts=(tcpsvr*)arg;
    while(1){
        string buff;
        ts->_recv(buff);
        cout<<"client say : "<<buff<<endl;
        cout<<"server say : ";
        buff="sever get it";
        ts->_send(buff);
    }
    ts->_close();
    delete ts;
}


int main(int argc,char* argv[]){
    if(argc!=3){
        return 0;
    }

    tcpsvr us;
    string ip;
    uint16_t port;

    ip=argv[1];
    port=atoi(argv[2]);

    if(!us._creat()){
        cout<<"socket error"<<endl;
        return 0;
    }
    if(!us._bind(ip,port)){
        cout<<"bind error"<<endl;
        return 0;
    }
    if(!us._listen(10)){  //已完成连接队列大小，这里随便取的
        cout<<"listen error"<<endl;
        return 0;
    }

    while(1){
        tcpsvr* link=new tcpsvr();
        struct sockaddr_in addr;
        if(!us._accept(*link,&addr)){
            continue;  //失败则继续阻塞
        }
        cout<<"we have a new connect from "<<inet_ntoa(addr.sin_addr)<<':'<<(int)port<<endl;

        pthread_t tid;
        int ret = pthread_create(&tid,NULL,threadstart,(void*)link);
        if(ret < 0){
            perror("creat thread");
            return false;
        }
    }
    us._close();
    return 0;
}
