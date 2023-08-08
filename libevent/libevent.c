//
// Created by 代文波 on 2023/7/19.
//
#include "libevent.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>

void testLibevent() {
    int i = 0;
    //获取当前系统支持的方法
    //获得当前系统(或者称为平台)支持的方法有哪些
    const char **p = event_get_supported_methods();
    while (p[i] != NULL) {
        printf("%s \t", p[i++]);
    }
    printf("\n");

    //获取地基接节点
    struct event_base *base = event_base_new();
    if (base == NULL) {
        printf("event_base_new error\n");
        return;
    }
    //获取当前系统调用的方法
    //获得当前base节点使用的多路io方法
    const char *pp = event_base_get_method(base);
    printf("%s\n", pp);

    //释放地基节点
    event_base_free(pp);
}
void readcb(evutil_socket_t fd, short events, void *arg){
    //struct event_base *base = (struct event_base *) arg;
    int n = 0;
    char buff[1024];
    memset(buff, 0x00, sizeof(buff));
    //读数据
    n = read(fd, buff, sizeof(buff));
    if (n <= 0) {
        printf("read error or cliant close n==[%d]\n", n);
        close(fd);
        //event_del();
        return;
    }
    printf("n==[%d] buf==[%s]\n", n, buff);

    for (int i = 0; i < n; i++) {
        buff[i] = toupper(buff[i]);
    }
    //发送数据
    write(fd, buff, n);
}
void conncb(evutil_socket_t fd, short events, void *arg){
    struct event_base *base = (struct event_base *) arg;
    //len 是一个输入输出参数
    int cfd = accept(fd, NULL, NULL);
    if (cfd>0){
        //负责创建event结构指针, 同时指定对应的地基base
        struct event *connev= event_new(base,cfd,EV_READ|EV_PERSIST,readcb,NULL);
        if (connev==NULL){
            exit(0);
        }
        //将通信文件描述符对应的事件上event_base 基地
        event_add(connev,NULL);
    }
}

void testLibeventSocket() {
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
    //创建地基
    struct event_base *base = event_base_new();
    if (NULL == base) {
        printf("event_base_new error\n");
        return;
    }
    //创建监听文件描述符对应事件
    struct event *event = event_new(base, lfd, EV_READ | EV_PERSIST, conncb, base);
    if (event==NULL){
        printf("event_new error\n");
        return;
    }
    //将新的事件节点添加上base 开始监听事件是否产生
    event_add(event,NULL);
    //进入循环等待事件的发生
    event_base_dispatch(base);

    event_base_free(base);
    event_free(event);
    close(lfd);

}