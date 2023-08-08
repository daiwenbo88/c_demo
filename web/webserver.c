//
// Created by 代文波 on 2023/7/22.
//
#include "webserver.h"
#include "pub.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <dirent.h>
#include "fcntl.h"
#include "unistd.h"
#include "../socket/wrap.h"
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <unistd.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <signal.h>

int send_header(int cfd, char *code, char *msg, char *fileType, int len) {
    char buf[1024];
    sprintf(buf, "HTTP/1.1 %s %s\r\n", code, msg);
    sprintf(buf + strlen(buf), "Content_Type:%s\r\n", fileType);
    if (len > 0) {
        sprintf(buf + strlen(buf), "Content_Length:%d\r\n",len);
    }
    strcat(buf, "\r\n");
    Write(cfd, buf, strlen(buf));
    return 0;
}

int send_file(int cfd, char *fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        perror("open file error");
        return -1;
    }
    char buf[1024];
    while (1) {
        memset(buf, 0, sizeof(buf));
        //读文件
        int n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            break;
        } else{
            write(cfd,buf,n);
        }
    }
    close(fd);
    return 1;
    //printf("n==[%d],buf==[%s]\n", n, buf);
}

//客户端有数据发送过来
void readMessageCallback(evutil_socket_t fd, short events, void *arg) {
    int n = 0;
    char buff[1024];
    memset(buff, 0x00, sizeof(buff));
    //读数据
    n = Readline(fd, buff, sizeof(buff));
    if (n <= 0) {
        printf("read error or cliant close n==[%d]\n", n);
        close(fd);
        //event_del();
        return;
    }
    char reqType[16] = {0};
    char fileName[255] = {0};
    char protocal[16] = {0};
    sscanf(buff, "%[^ ] %[^ ] %[^ \r\n]", reqType,fileName,protocal);
    printf("n==[%d] reqType==[%s] fileName=[%s] protocal=[%s]\n", n, reqType, fileName, protocal);
    //循环读取数据 避免产生粘包
    while((n= Readline(fd,buff,sizeof(buff)))>0);

    char *pFile=fileName + 1;
    if (strlen(fileName)<=1){
        strcpy(pFile,"./");
    }
    //转换汉子编码
    strdecode(pFile,pFile);
    printf("filename=  %s\n",pFile);
    struct stat st;
    //判断文件是否存在
    if(stat(pFile,&st)<0){
        printf("file not exist\n");
        send_header(fd,"404","NOT FOUND",get_mime_type(".html"),0);
        send_file(fd,"error.html");
    } else{
        if (S_ISREG(st.st_mode)){//普通文件
            send_header(fd,"200","SUCCESS",get_mime_type(pFile),st.st_size);
            send_file(fd,pFile);
        } else if(S_ISDIR(st.st_mode)){//目录文件
            send_header(fd,"200","SUCCESS",get_mime_type(".html"),0);
            char buffer[1024];
            send_file(fd,"html/dir_header.html");
            struct  dirent **nameList;
            int n = scandir(pFile,&nameList,NULL,alphasort);
            if (n<0){
                perror("scandir");
                close(fd);
            } else{
                while (n--){
                    printf("%s\n",nameList[n]->d_name);
                    memset(buffer, 0x00, sizeof(buffer));
                    if (nameList[n]->d_type==DT_DIR){
                        sprintf(buffer,"<li><a href=%s/>%s</a></li>",nameList[n]->d_name, nameList[n]->d_name);
                    } else {
                        sprintf(buffer, "<li><a href=%s>%s</a></li>", nameList[n]->d_name, nameList[n]->d_name);
                    }
                    free(nameList[n]);
                    Write(fd,buffer, strlen(buffer));
                }
                free(nameList);
                send_file(fd,"html/dir_tail.html");
            }
        }

    }

//    printf("n==[%d] buf==[%s]\n", n, buff);
//
//    for (int i = 0; i < n; i++) {
//        buff[i] = toupper(buff[i]);
//    }
//    //发送数据
//    write(fd, buff, n);
}

//请求连接回调
void connectCallback(evutil_socket_t fd, short events, void *arg) {
    struct event_base *base = (struct event_base *) arg;
    int cfd = accept(fd, NULL, NULL);
    if (cfd > 0) {
        //设置文件为非阻塞
        int flag = fcntl(cfd,F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(cfd,F_SETFL,flag);

        struct event *connev = event_new(base, cfd, EV_READ | EV_PERSIST, readMessageCallback, NULL);
        if (connev == NULL) {
            exit(0);
        }
        event_add(connev, NULL);
    }
}

void testWebServer() {
    //web 服务器给浏览器发送数据的时候 浏览器已经关闭连接
    //则web会收到SIG_IGN 信号
    //忽略SIG_IGN 信号
//    struct sigaction act;
//    act.sa_handler = SIG_IGN;
//    sigemptyset(&act.sa_mask);
//    act.sa_flags=0;
//    sigaction(SIGPIPE,&act,NULL);
    //忽略SIG_IGN 信号
    signal(SIGPIPE,SIG_IGN);
    //改变当前进程的工作目录
    char path[255] = {0};
    sprintf(path, "%s/%s", getenv("HOME"), "Desktop/C++/ipc/web/webpath");
    chdir(path);
    printf("path=%s \n",path);


    //创建socket --设置端口复用 --bind;
    int lfd = tcp4bind(9999, NULL);
    //设置监听
    Listen(lfd, 128);
    //创建base 基地
    struct event_base *base = event_base_new();
    if (NULL == base) {
        printf("event_base_new error\n");
        return;
    }
    //创建监听文件描述符对应事件
    struct event *event = event_new(base, lfd, EV_READ | EV_PERSIST, connectCallback, base);
    if (NULL == event) {
        printf("event_new error\n");
        return;
    }
    //将新的事件节点添加上base基地 开始监听事件是否产生
    event_add(event, NULL);
    //进入循环等待事件发生
    event_base_dispatch(base);

    event_base_free(base);
    event_free(event);
    close(lfd);
}
void testScandir(){
    struct  dirent **nameList;
    int n = scandir("/Users/daiwenbo/Desktop/C++/ipc/web/webpath",&nameList,NULL,alphasort);
    if (n<0){
        perror("scandir");
    } else{
        while (n--){
            printf("%s\n",nameList[n]->d_name);
            free(nameList[n]);
        }
        free(nameList);
    }
}
