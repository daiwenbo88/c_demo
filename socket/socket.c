//
// Created by 代文波 on 2023/7/13.
//
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "wrap.h"

//服务端程序开发
void testServSocket() {
    //创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("socket error");
        return;
    }
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //绑定
    int ret = bind(lfd, (struct sockaddr *) &serv, sizeof(serv));
    if (ret < 0) {
        perror("bind error");
        return;
    }
    //监听
    listen(lfd, 5);
    //获得一个连接, 若当前没有连接则会阻塞等待
    //accept函数是一个阻塞函数, 若没有新的连接请求, 则一直阻塞.
    //从已连接队列中获取一个新的连接, 并获得一个新的文件描述符,
    // 该文件描述符用于和客户端通信.
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    //len 是一个输入输出参数
    int cfd = accept(lfd, (struct sockaddr *) &client, &len);
    //获取客户端ip地址
    char SIP[16];
    memset(SIP, 0x00, sizeof(SIP));
    printf("ip=%s\n", inet_ntop(AF_INET, &client.sin_addr.s_addr,
                                SIP, sizeof(SIP)));
    printf("ROPT =%d \n", ntohs(client.sin_port));
    printf("lfd==[%d],cfd==[%d]\n", lfd, cfd);

    if (cfd < 0) {
        perror("accept error");
        return;
    }
    int n = 0;
    char buff[1024];
    while (1) {
        memset(buff, 0x00, sizeof(buff));
        //读数据
        n = read(cfd, buff, sizeof(buff));
        if (n <= 0) {
            printf("read error or cliant close n==[%d]\n", n);
            break;
        }
        printf("n==[%d] buf==[%s]\n", n, buff);

        for (int i = 0; i < n; i++) {
            buff[i] = toupper(buff[i]);
        }
        //发送数据
        write(cfd, buff, n);
    }
    close(lfd);
    close(cfd);
}

void testClientSocket() {
    //创建socket --用于和服务端通信
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd < 0) {
        perror("socket error");
        return;
    }
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);//端口设置
    //serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //设置ip地址
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
    //链接服务器
    int ret = connect(cfd, (struct sockaddr *) &serv, sizeof(serv));
    if (ret < 0) {
        perror("connect error");
        return;
    }
    int n = 0;
    char buf[1024];

    while (1) {
        memset(buf, 0x00, sizeof(buf));
        //从标准输入读数据
        n = read(STDIN_FILENO, buf, sizeof(buf));
        //往发送缓存区写数据
        write(cfd, buf, n);

        memset(buf, 0x00, sizeof(buf));
        //读服务端数据
        n = read(cfd, buf, sizeof(buf));
        if (n <= 0) {
            printf("read error or cliant close n==[%d]\n", n);
            break;
        }
        printf("n==[%d] buf==[%s]\n", n, buf);
    }
    close(cfd);
}

void processServSocket() {
    //AF_INET IPV4
    //SOCK_STREAM 流式
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //将socket文件描述符和IP,PORT绑定
    Bind(lfd, (struct sockaddr *) &serv, sizeof(serv));
    //将套接字由主动态变为被动态
    Listen(lfd, 3);
    int cfd;
    pid_t pid;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    while (1) {
        //获得一个连接, 若当前没有连接则会阻塞等待.
        //addr: 传出参数, 保存客户端的地址信息
        cfd = Accept(lfd, (struct sockaddr *) &client, &len);
        //创建子进程
        pid = fork();
        if (pid < 0) {
            perror("fork error");
            return;
        }
        if (pid > 0) {//父进程
            close(cfd);
        } else {//子进程
            close(lfd);
            //获取客户端ip地址
            char SIP[16];
            memset(SIP, 0x00, sizeof(SIP));
            printf("ip=%s\n", inet_ntop(AF_INET, &client.sin_addr.s_addr,
                                        SIP, sizeof(SIP)));
            int n = 0;
            char buff[1024];
            int i;
            while (1) {
                memset(buff, 0x00, sizeof(buff));
                //读数据
                n = read(cfd, buff, sizeof(buff));
                if (n <= 0) {
                    printf("read error or client close n==[%d]\n", n);
                    break;
                }
                printf("ROPT =%d n==[%d] buf==[%s]\n", ntohs(client.sin_port), n, buff);

                for (i = 0; i < n; i++) {
                    buff[i] = toupper(buff[i]);
                }
                //发送数据
                write(cfd, buff, n);
            }
            close(cfd);

            exit(0);
        }
        close(cfd);
    }

}

void *thread_work(void *arg) {
    int cfd = *(int *) arg;
    int n = 0;
    char buff[1024];
    int i;
    while (1) {
        memset(buff, 0x00, sizeof(buff));
        //读数据
        n = read(cfd, buff, sizeof(buff));
        if (n <= 0) {
            printf("read error or client close n==[%d]\n", n);
            break;
        }
        printf("n==[%d] buf==[%s]\n", n, buff);
        for (i = 0; i < n; i++) {
            buff[i] = toupper(buff[i]);
        }
        //发送数据
        write(cfd, buff, n);
    }
    close(cfd);
    pthread_exit(NULL);
}

void threadServSocket() {
    //AF_INET IPV4
    //SOCK_STREAM 流式
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //将socket文件描述符和IP,PORT绑定
    // \续行符
    Bind(lfd, (struct sockaddr *) &serv, \
    sizeof(serv));
    //将套接字由主动态变为被动态
    Listen(lfd, 3);
    int cfd;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    pthread_t thread;
    while (1) {
        //获得一个连接, 若当前没有连接则会阻塞等待.
        //addr: 传出参数, 保存客户端的地址信息
        cfd = Accept(lfd, (struct sockaddr *) &client, &len);
        //创建子进程
        pthread_create(&thread, NULL, thread_work, &cfd);
        //设置子线程为分离属性
        pthread_detach(thread);
    }
    close(lfd);
}

void selectWork(int *sockfd) {
    int cfd = *sockfd;
    int n = 0;
    char buff[1024];
    int i;
    while (1) {
        memset(buff, 0x00, sizeof(buff));
        //读数据
        n = read(cfd, buff, sizeof(buff));
        if (n <= 0) {
            printf("read error or client close n==[%d]\n", n);
            break;
        }
        printf("n==[%d] buf==[%s]\n", n, buff);
        for (i = 0; i < n; i++) {
            buff[i] = toupper(buff[i]);
        }
        //发送数据
        write(cfd, buff, n);
    }
    close(cfd);
    pthread_exit(NULL);
}


void testSelectSocket() {
    //AF_INET IPV4
    //SOCK_STREAM 流式
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //将socket文件描述符和IP,PORT绑定
    // \续行符
    Bind(lfd, (struct sockaddr *) &serv, sizeof(serv));
    //将套接字由主动态变为被动态
    Listen(lfd, 3);
    fd_set readfds;
    fd_set tempfds;

    FD_ZERO(&readfds);
    FD_ZERO(&tempfds);

    FD_SET(lfd, &readfds);
    int nready;
    int maxfd = lfd;
    int cfd, sockfd;
    while (1) {
        tempfds = readfds;
        //tempfd是输入输出参数
        //输入：告诉内核要监测那些文件描述符
        //输出：内核告诉应用程序有哪些文件描述符发生了变化
        nready = select(maxfd + 1, &tempfds, NULL, NULL, NULL);
        //nready 返回发生变化的文件描述符的个数
        if (nready < 0) {
            if (errno == EINTR) {//被信号中断
                continue;
            }
            break;
        }
        printf("nready==[%d]\n",nready);
        //有客户端连接请求到来
        if (FD_ISSET(lfd, &tempfds)) {
            //接受新的客户端连接请求
            cfd = Accept(lfd, NULL, NULL);
            FD_SET(cfd, &readfds);
            if (maxfd < cfd) {
                maxfd = cfd;
            }
            //没有其他客户端 发送数据过来
            if (--nready == 0) {
                continue;
            }
        }
        //有数据发来情况
        for (int i = lfd + 1; i <= maxfd; i++) {
            sockfd = i;
            //判断sockfd 文件描述符是否有变化
            if (FD_ISSET(sockfd, &tempfds)) {
                int n = 0;
                char buff[1024];
                memset(buff, 0x00, sizeof(buff));
                //读数据
                n = read(sockfd, buff, sizeof(buff));
                if (n <= 0) {
                    printf("read error or client close n==[%d]\n", n);
                    close(sockfd);
                    //移除
                    FD_CLR(sockfd,&readfds);
                } else{
                    int k;
                    printf("n==[%d] buf==[%s]\n", n, buff);
                    for (k = 0; k < n; k++) {
                        buff[i] = toupper(buff[i]);
                    }
                    //发送数据
                    write(cfd, buff, n);
                }
                //没有其他客户端 发送数据过来
                if (--nready == 0) {
                    break;
                }
            }
        }


    }

}

void testPollSocket(){
    //AF_INET IPV4
    //SOCK_STREAM 流式
    int lfd = Socket(AF_INET, SOCK_STREAM, 0);
    //设置端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    //本地服务器的IP地址和PORT,
    struct sockaddr_in serv;
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);//表示本机可用任意ip
    //将socket文件描述符和IP,PORT绑定
    // \续行符
    Bind(lfd, (struct sockaddr *) &serv, sizeof(serv));
    //将套接字由主动态变为被动态
    Listen(lfd, 3);

    //epoll_create(1024);

    close(lfd);
}