//
// Created by 代文波 on 2023/7/8.
//
#include "fifo.h"
#include "stdio.h"
#include "string.h"
#include <sys/stat.h>
#include "unistd.h"
#include <fcntl.h>

void testFifoWrite() {
    //创建 fifo 文件
    int ret = access("./myfifo",F_OK);
    if(ret!=0){
        int rest = mkfifo("./myfifo", 0777);
        if (rest < 0) {
            perror("mkfifo error");
            return;
        }
    }
    //打开文件
    int fd = open("./myfifo",O_RDWR);
    if (fd < 0) {
        perror("open error");
        return;
    }
    //写入文件
    //char *str="hello world";
    write(fd,"hello world", strlen("hello world"));
    //关闭文件
    close(fd);
    //getchar();
}

void testFifoRead(){
    //打开文件
    int fd = open("./myfifo",O_RDWR);
    if (fd < 0) {
        perror("open error");
        return;
    }
    char buf[64];
    memset(buf,0,sizeof(buf));
    read(fd,buf,sizeof(buf));
    printf("read buf= %s\n",buf);
    //关闭文件
    close(fd);
    //getchar();
}