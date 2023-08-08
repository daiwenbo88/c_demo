//
// Created by 代文波 on 2023/7/17.
//
#include "local_socket.h"
#include "stdio.h"
#include "string.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <stddef.h>


//本地通信

void testLocalServerSocket(){
    //创建socket AF_UNIX 本地通信
    int lfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("socket error");
        return;
    }
    //先删除通信文件
    unlink("./serv.sock");
    //本地服务器的IP地址和PORT,
    struct sockaddr_un serv;
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path,"./serv.sock");
    //绑定
    int ret = bind(lfd, (struct sockaddr *) &serv, sizeof(serv));
    if (ret < 0) {
        perror("bind error");
        return;
    }
    //监听
    listen(lfd, 5);

    struct sockaddr_un client;
    socklen_t len = sizeof(client);
    //len 是一个输入输出参数
    int cfd = accept(lfd, (struct sockaddr *) &client, &len);
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

void testLocalClientSocket(){
    //创建socket AF_UNIX 本地通信
    int lfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("socket error");
        return;
    }
    //链接服务端
    struct sockaddr_un client;
    client.sun_family=AF_UNIX;
    strcpy(client.sun_path,"./serv.sock");
    int ret = connect(lfd, (struct sockaddr *) &client, sizeof(client));
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
        write(lfd, buf, n);

        memset(buf, 0x00, sizeof(buf));
        //读服务端数据
        n = read(lfd, buf, sizeof(buf));
        if (n <= 0) {
            printf("read error or cliant close n==[%d]\n", n);
            break;
        }
        printf("n==[%d] buf==[%s]\n", n, buf);
    }
    close(lfd);
}

void testOffsets(){
    struct  s{
        int i;
        char c;
        double d;
        char a[1];
    };
    printf("offsets: i=%zd; c=%zd; d=%zd; z=%zd\n",
           offsetof(struct s,i),offsetof(struct s,c),
           offsetof(struct s,d), offsetof(struct s,a));
    printf("sizeof(struct s)=%zd\n",sizeof(struct s));
}