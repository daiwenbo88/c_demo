//
// Created by 代文波 on 2023/7/9.
//
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/types.h"
#include <sys/time.h>
#include <sys/stat.h>
#include "unistd.h"
#include <signal.h>
#include <time.h>
#include <fcntl.h>

void sighandler(int signo) {
    printf("signo==[%d]\n", signo);
}

//给没有读端的管道写数据 会产生SIGPIPE 信号
void testSignal() {
    int fd[2];
    //创建管道
    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe error");
        return;
    }
    //注册SIGPIPE信号处理函数
    signal(SIGPIPE, sighandler);
    //关闭读端
    close(fd[0]);
    write(fd[1], "hello world", strlen("hello world"));
}

void testSignal2() {
    //给指定进程发送指定信号
    //kill(getppid(),SIGKILL);

    //给当前线程发送SIGKILL
    //raise(SIGKILL);

    //给自己发送异常终止信号
    //abort();

    //在指定seconds后，内核会给当前进程发送14）SIGALRM信号
    alarm(5);

    printf("hello world\n");
}

void testSignal3() {
    signal(SIGALRM, sighandler);

    int n = alarm(5);
    printf("n==[%d]\n", n);

    sleep(2);
    n = alarm(5);
    printf("n==[%d]\n", n);

    sleep(10);
}

void testAlarm() {
    int i = 0;
    alarm(1);
    while (1) {
        printf("i=%d \n", i++);
    }
}

void testSetitTimer() {
    //注册信号处理函数
    signal(SIGALRM, sighandler);

    struct itimerval tim;
    // 周期性赋值  设定第一次执行function所延迟的秒数
    struct timeval interval;
    interval.tv_sec = 1;//秒
    interval.tv_usec = 0;//微妙

    // 第一次触发赋值 设定以后每几秒执行function
    struct timeval timeValue;
    timeValue.tv_sec = 3;//秒
    timeValue.tv_usec = 0;//微妙

    //第一次3秒后执行 后面每隔1秒执行一次
    tim.it_interval = interval;//闹钟触发周期
    tim.it_value = timeValue;//闹钟触发时间

    //注册定时器
    setitimer(ITIMER_REAL, &tim, NULL);

    while (1) {
        sleep(1);
    }
}

void testSigaction() {
    //注册SIGINT 信号处理函数
    struct sigaction action;
    action.__sigaction_u.__sa_handler = sighandler;
    sigemptyset(&action.sa_mask);//阻塞的信号
    sigaddset(&action.sa_mask, SIGQUIT);//
    action.sa_flags = 0;
    //control+C 发送 SIGINT
    signal(SIGQUIT, sighandler);
    sigaction(SIGINT, &action, NULL);

    while (1) {
        sleep(2);
    }
}

void waitchild(int signo) {
    int status;
    //WNOHANG 非阻塞
    pid_t wpid;
    while (1) {
        wpid = waitpid(-1, &status, WNOHANG);
        if (wpid > 0) {
            if (WIFEXITED(status)) {//进程正常终止
                printf("进程=%d 正常退出 %d\n", wpid, WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status)) {//进程异常终止
                printf("异常退出 %d\n", WTERMSIG(status));
            }
        } else if (wpid < 0) {
            printf("无子进程\n");
            break;
        } else { //==0 子进程还活着
            printf("child is living .wpid===[%d]\n", wpid);
            break;
        }
    }
}

//  SIGCHLD子进程退出 信号
void testSigchld() {
    pid_t pid;
    int i = 0;
    //将SIGCHLD信号阻塞
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);//信号加入信号集合中
    sigprocmask(SIG_BLOCK, &mask, NULL);//屏蔽(阻塞)信号

    for (i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork error");
            return;
        }
        if (pid > 0) {
            printf("print process pid=[%d] child pid=[%d]\n", getppid(), pid);
        } else {
            printf("child process pid=[%d] child pid=[%d]\n", getppid(), pid);
            break;
        }
    }
    if (i == 3) {
        struct sigaction action;
        action.sa_handler = waitchild;
        //action.__sigaction_u.__sa_handler = waitchild;
        sigemptyset(&action.sa_mask);//阻塞的信号
        action.sa_flags = 0;
        sleep(5);
        //捕获SIGCHLD(信号) 进程退出信号
        sigaction(SIGCHLD, &action, NULL);
        //解除对SIGCHLD信号的阻塞
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        while (1) {
            sleep(1);
        }
    }
}

int num = 0;
int flag;

void func1(int signo) {
    printf("F:[%d]\n", num);
    num += 2;
    flag = 0;
    sleep(1);
}

void func2(int signo) {
    printf("F:[%d]\n", num);
    num += 2;
    flag = 0;
    sleep(1);
}

void testSigchld2() {
    int ret;
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }
    if (pid > 0) {
        num = 0;
        flag = 1;
        //注册SIGUSR1
        signal(SIGUSR1, func1);
        while (1) {
            if (flag == 0) {
                //给子进程发送SIGUSR2
                kill(pid, SIGUSR2);
                flag = 1;
            }
        }
    } else {
        num = 1;
        flag = 0;
        //注册SIGUSR2
        signal(SIGUSR2, func2);
        while (1) {
            if (flag == 0) {
                kill(getppid(), SIGUSR1);
                flag = 1;
            }
        }
    }
}

void myfunc(int signo) {
    //获取文件描述符
    int fd = open("mydemo.log", O_RDWR | O_CREAT|O_APPEND,0755);
    if (fd < 0) {
        return;
    }
    //获取当前的系统时间
    time_t t;
    char *p = ctime(&t);
    write(fd,p, strlen(p));
    close(fd);
}

void testDaemon() {
    //父进程fork 子进程 然后父进程退出
    pid_t pid = fork();
    if (pid < 0 || pid > 0) {
        exit(1);
    }
    //子进程调用setsid函数创建会话
    setsid();
    //改变当前工作目录
    chdir("./log");
    //改变文件掩码
    umask(0000);
    //关闭输入输出 、错误文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //注册SIGALRM信号处理函数
    struct sigaction act;
    act.sa_handler = myfunc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    //设置时钟
    struct itimerval time;
    time.it_value.tv_sec = 2;
    time.it_value.tv_usec = 0;

    time.it_interval.tv_sec = 3;
    time.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &time, NULL);
    printf("hello world");
    while (1) {
        sleep(2);
    }
}