//
// Created by 代文波 on 2023/7/7.
//
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "pipe.h"
#include <fcntl.h>

void testPipe() {
    int fd[2];
    //创建管道 fd[0]存放管道的读端，fd[1]存放管道的写端
    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe error");
        return;
    }
    //创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }

    if (pid>0){//子进程pid
        close(fd[0]);//关闭读端
        sleep(3);
        write(fd[1],"hello world", strlen("hello wold"));
        wait(NULL);
    } else{
        close(fd[1]);//关闭写端
        char buf[64];
        memset(buf,0,sizeof(buf));
        //read 在没有数据可读时是阻塞的
        long n = read(fd[0],buf,sizeof(buf));
        printf("read over n=[%ld] buf=[%s]",n,buf);
    }

}

void testPipe2(){
    int fd[2];
    //创建管道 fd[0]存放管道的读端，fd[1]存放管道的写端
    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe error");
        return;
    }
    //创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }

    if (pid>0){//父进程pid
        int status;
        pid_t wpid = wait(&status);//如果子进程没有退出 这个函数会阻塞等待
        if (wpid>0){
            printf("wpid=[%d]\n",wpid);
            if (WIFEXITED(status)){//进程正常终止
                printf("正常退出 %d\n",WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status)){//进程异常终止
                printf("异常退出 %d\n",WTERMSIG(status));
            }
        }
        //关闭读端
        close(fd[0]);
        //将标准输出重定向到管道写端
        dup2(fd[1],STDOUT_FILENO);
        execlp("ps","ps" ,"aux" ,NULL);
        perror("execlp error");

    } else{
        //子进程
        close(fd[1]);//关闭读端
        //将标准输入重定向到管道读端
        dup2(fd[0],STDIN_FILENO);
        //一个进程里面执行其他的命令
        execlp("ps","grep"," bash",NULL);
        perror("execlp error");
    }
}

void testPipe3(){
    int fd[2];
    //创建管道 fd[0]存放管道的读端，fd[1]存放管道的写端
    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe error");
        return;
    }
    //设置管道读端为非阻塞
    int flags = fcntl(fd[0],F_GETFL,0);
    flags|=O_NONBLOCK;
    fcntl(fd[0],F_SETFL,flags);

    //设置管道写端为非阻塞
    flags = fcntl(fd[1],F_GETFL,0);
    flags|=O_NONBLOCK;
    fcntl(fd[1],F_SETFL,flags);

    char buf[64];
    memset(buf,0,sizeof(buf));
    //read 在没有数据可读时是阻塞的
    long n = read(fd[0],buf,sizeof(buf));
    printf("read over n=[%ld] buf=[%s]",n,buf);
    //获取管道大小
    printf("pipe size==[%ld]\n", fpathconf(fd[0], _PC_PIPE_BUF));
}