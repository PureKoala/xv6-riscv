
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes_cal(int *p){
    int prime;
    // 从管道中读取一个数
    if(read(p[0], &prime, sizeof(int)) == 0){
        return;
    }

    printf("prime %d\n", prime);

    int p2[2];
    // 创建一个新的管道
    pipe(p2);

    // 创建子进程
    if(fork() == 0){
        // 关闭父进程的读端
        close(p[0]);
        // 关闭子进程的写端
        close(p2[1]);
        // 调用primes_cal函数
        primes_cal(p2);
        // 关闭子进程的读端
        close(p2[0]);
        exit(0);
    }
    else{
        // 关闭读端
        close(p2[0]);
        // 读取管道中的数
        int n;
        while(read(p[0], &n, sizeof(int)) > 0){
            // 如果数不能被质数整除，则写入新的管道
            if(n % prime != 0){
                write(p2[1], &n, sizeof(int));
            }
        }
        // 关闭写端
        close(p[0]);
        close(p2[1]);
        // 等待子进程结束
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]){
    int p[2];
    // 创建一个管道
    pipe(p);
    
    
    // 将3到35之间的数写入管道
    if(fork() == 0){
        // 关闭父进程的写端
        close(p[1]);
        // 调用primes_cal函数
        primes_cal(p);
        // 关闭父进程的读端
        close(p[0]);
        exit(0);
    } else {
        // 关闭子进程的读端
        close(p[0]);
        // 将3到35之间的数写入管道
        for(int i = 2; i < 36; i++){
            write(p[1], &i, sizeof(int));
        }
        // 关闭子进程的写端
        close(p[1]);
        // 等待子进程结束
        wait(0);
        exit(0);
    }
    return 0;

}