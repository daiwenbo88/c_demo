//
// Created by 代文波 on 2023/7/16.
//
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void testUdpServer(){
    //创建socket UDP
    int lfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    int n;
    socklen_t  len;
    char buf[1024];
    struct sockaddr_in client;
    while (1){
        memset(buf,0x00,sizeof (buf));
        len = sizeof (client);
        //读取数据
        n = recvfrom(lfd,buf,sizeof (buf),0,(struct sockaddr *)&client,&len);

        for(int i=0;i<n;i++){
            buf[i]= toupper(buf[i]);
        }
        printf("buf= %s, n= %d\n",buf,n);
        //发送数据
        sendto(lfd,buf,n,0,(struct sockaddr *)&client,len);
    }
    close(lfd);
}

void testUdpClient(){
    //创建socket UDP
    int lfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (lfd < 0) {
        perror("socket error");
        return;
    }
    int n;
    char buf[1024];
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    //设置ip地址
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);
    while (1){
        memset(buf,0x00,sizeof (buf));
        n=read(STDIN_FILENO,buf,sizeof(buf));
        //发送数据
        sendto(lfd,buf,n,0,(struct sockaddr *)&serv,sizeof(serv));

        //读取数据
        memset(buf, 0x00, sizeof(buf));
        recvfrom(lfd,buf,sizeof (buf),0,NULL,NULL);
        printf("buf= %s, n= %d",buf,n);
    }
    close(lfd);
}