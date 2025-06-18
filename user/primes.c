#include "kernel/types.h"
#include "user.h"

__attribute__((noreturn))
void sieve(int fd){
    int p;
    int n;
    int pipe1[2];

    if(read(fd, &p, sizeof(int)) != sizeof(int)){
        close(fd);
        exit(0);
    }

    printf("prime %d\n", p);
    pipe(pipe1);
    if (fork() == 0) {
        // 子进程递归处理
        close(pipe1[1]);      // 关闭写端
        sieve(pipe1[0]);
        exit(0);
    } else {
        // 当前进程过滤 p 的倍数
        close(pipe1[0]);      // 关闭读端
        while (read(fd, &n, sizeof(int)) == sizeof(int)) {
            if (n % p != 0) {
                write(pipe1[1], &n, sizeof(int));
            }
        }
        close(fd);
        close(pipe1[1]);
        wait(0);  // 等子进程完成
        exit(0);
    }
}

int main(int argc,char* argv[]){
    if(argc > 1){
        printf("primes needs none argument!\n"); //检查参数数量是否正确
        exit(-1);
    }
    int p[2];
    pipe(p);
     if (fork() == 0) {
        // 子进程开始 sieve
        close(p[1]);
        sieve(p[0]);
        exit(0);
    } else {
        // 父进程写入初始数据
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}

