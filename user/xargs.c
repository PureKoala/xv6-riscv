#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

char *
readLine(){
    char *buf = malloc(sizeof(char) * MAXARG);
    char *pos = buf;

    while(read(0, pos, sizeof(char)) != 0){
        if(*pos == '\n' || *pos == '\0'){
            *pos = 0;
            return buf;
        }
        pos ++;
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    // 检查参数个数是否小于2
    if (argc < 2) {
        // 如果小于2，输出使用说明并退出
        fprintf(2, "Usage: xargs <command> [args...]\n");
        exit(1);
    }

    char **args;
    args = malloc(sizeof(char *) * argc);

    for (int i = 0; i < argc - 1; i++) {
        // args[i] = strcpy(args[i], argv[1+i]);
        args[i] = argv[1+i];
    }


    char* temp;
    while ((temp = readLine()) != 0) {
        if (fork() == 0) {
            // 在子进程中执行命令
            args[argc - 1] = temp;
            exec(argv[1], args);
            // 子进程退出
            exit(0);
        } else
            wait(0);
    }
    
    return 0;
}