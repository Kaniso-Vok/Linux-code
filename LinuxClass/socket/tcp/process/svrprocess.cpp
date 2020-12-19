#include "tcpsvr.hpp"
#include <sys/wait.h>
#include <signal.h>


void sigcome(int signo){
    (void)signo;
    while(1){
        waitpid(-1,NULL,WNOHANG);     //等待任意子进程退出信号
    }
}

int main(int argc,char* argv[]){
    if(argc!=3){
        return 0;
    }

    signal(SIGCHLD,sigcome);

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
        tcpsvr link;
        struct sockaddr_in addr;
        if(!us._accept(link,&addr)){
            continue;  //失败则继续阻塞
        }
        cout<<"we have a new connect from "<<inet_ntoa(addr.sin_addr)<<':'<<(int)port<<endl;

        //多进程
        int pid=fork();
        if(pid < 0){
            perror("fork error");
            return 0;
        }
        else if(pid == 0){
            //child
            while(1){
                string buff;
                link._recv(buff);
                cout<<"client say : "<<buff<<endl;
                cout<<"server say : ";
                buff.clear();
                //getline(cin,buff);若是输入返回的话由于只有一个stdout，一次收到多条客户端信息时容易导致回复逻辑混乱
                buff="sever already get it";
                link._send(buff);
            }
            link._close();
            exit(1);
        }
        //father
        link._close();
    }
    us._close();
    return 0;
}
