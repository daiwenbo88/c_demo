//
// Created by 代文波 on 2023/7/8.
//
#include "mmap.h"
#include "stdio.h"
#include "string.h"
#include <sys/mman.h>
#include <sys/wait.h>
#include "unistd.h"
#include <fcntl.h>

void testMmap() {
    //使用mmap 函数建立共享映射区
    int fd = open("./test.log", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        perror("open error");
        return;
    }

    int len = lseek(fd, 0, SEEK_END);
    //printf("len= %d",len);
    //int SIZE = 4096;
    void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr ==MAP_FAILED){
        perror("open mmap error");
        return;
    }
    //创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }
    if (pid > 0) {//子进程pid
        memcpy(addr,"hello world", strlen("hello world"));
        wait(NULL);
    } else {
        sleep(2);
        char *p = (char *)addr;
        printf("[%s]",p);
        close(fd);
    }
}

void testMmap2(){
    //使用mmap 函数建立共享映射区
    int fd = open("./test.log", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        perror("open error");
        return;
    }

    int len = lseek(fd, 0, SEEK_END);
    printf("len= %d",len);
    //int SIZE = 4096;
    void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr ==MAP_FAILED){
        perror("open mmap error");
        return;
    }
    memcpy(addr,"hello world", strlen("hello world"));
    //close(fd);
    getchar();
    munmap(addr,len);
}

void testMmap3(){
    //使用mmap 函数建立共享映射区
    int fd = open("./test.log", O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        perror("open error");
        return;
    }

    int len = lseek(fd, 0, SEEK_END);
    printf("len= %d",len);
    //int SIZE = 4096;
    void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr ==MAP_FAILED){
        perror("open mmap error");
        return;
    }
    char *p = (char *)addr;
    printf("[%s]",p);
    close(fd);
    munmap(addr,len);
}
//mmap 匿名映射
void testMmap4(){
    //mmap函数建立匿名映射
    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if(addr ==MAP_FAILED){
        perror("open mmap error");
        return;
    }
    //创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }
    if (pid > 0) {//子进程pid
        memcpy(addr,"hello world", strlen("hello world"));
        wait(NULL);
    } else {
        sleep(2);
        char *p = (char *)addr;
        printf("[%s]",p);
        munmap(addr,4096);
    }
}