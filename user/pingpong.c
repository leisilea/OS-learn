#include "kernel/types.h"
#include "user.h"

int main(int argc,char* argv[]){
    if(argc > 1){
        printf("pingpong needs none argument!\n"); //检查参数数量是否正确
        exit(-1);
    }
    int p2c[2];
    int c2p[2];
    char buf[10];
    /* 子进程读管道，父进程写管道 */
    pipe(p2c);
    pipe(c2p); 
    int pid = fork();
    if (pid == 0) { 
        /* 子进程 */
        //接受ping
        close(c2p[0]);
        close(p2c[1]); // 关闭写端
        
        read(p2c[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        close(p2c[0]); // 读取完成，关闭读端
        //发送pong
        strcpy(buf, "pong");
        write(c2p[1], buf, 4);
        close(c2p[1]);
        exit(0); //确保进程退出
    } else if (pid>0) { 
        /* 父进程 */
        
        close(p2c[0]); // 关闭读端
        close(c2p[1]);
        strcpy(buf, "ping");
        write(p2c[1], buf, 4);
        close(p2c[1]); // 写入完成，关闭写端
        read(c2p[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        close(c2p[0]);
        exit(0); //确保进程退出
    }
    
}

