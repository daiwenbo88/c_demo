//
// Created by 代文波 on 2023/7/11.
//
#include <pthread.h>
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
#include "thread.h"

struct Person {
    int age;
    char name[20];
};

void *mythread(void *arg) {
    //arg 创建线程传递的参数
    //int n = *(int *) arg;
    struct Person *p = (struct Person *) arg;
    printf("age = [%d] name = [%s]\n", p->age, p->name);
    printf("child pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());
}


void testCreateThread() {
    pthread_t thread;
    int n = 99;

    struct Person p;
    memset(&p, 0x00, sizeof(struct Person));
    p.age = 20;
    strcpy(p.name, "张三");

    int ret = pthread_create(&thread, NULL, mythread, &p);
    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }
    printf("main pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());
    sleep(2);//为了让子线程能够 拉起来
}

void *threadCllback(void *arg) {
    //arg 创建线程传递的参数
    int n = *(int *) arg;
    printf("child pid=[%d] thread id=[%ld] thread num=[%d]\n", getpid(), pthread_self(), n);
}

void testCreateThread2() {
    int i, n;
    n = 5;
    pthread_t thread[n];
    int arr[5];
    for (i = 0; i < n; i++) {
        arr[i] = i;
        int ret = pthread_create(&thread[i], NULL, threadCllback, &arr[i]);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }
    }

    printf("main pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());
    sleep(2);//为了让子线程能够 拉起来
}

int g_var = 9;

void *threadExit(void *arg) {
    //arg 创建线程传递的参数
    printf("child pid=[%d] thread id=[%ld] thread num=[%d]\n", getpid(), pthread_self(), 10);
    //退出子线程
    pthread_exit(&g_var);
    //另注意，pthread_exit或者return返回的指针所指向的内存单元必须是全局的或者是用malloc分配的，
    //不能在线程函数的栈上分配，因为当其它线程得到这个返回指针时线程函数已经退出了，
    // 栈空间就会被回收
}

void testCreateThread3() {
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, threadExit, NULL);
    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }
    printf("main pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());
    sleep(5);//为了让子线程能够 拉起来
    void *p = NULL;
    pthread_join(thread, &p);
    //&p值 是pthread_exit(&g_var); &g_var的返回值
    int n = *(int *) p;
    printf("child exit status :[%d] [%p]", n, p);
}

void testCreateThread4() {
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, threadExit, NULL);
    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }
    printf("main pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());

    //设置线程为分离属性
    pthread_detach(thread);
    sleep(5);//为了让子线程能够 拉起来
    //线程为分离属性 则pthread_join 不再阻塞立即返回
    int result = pthread_join(thread, NULL);
    if (result != 0) {
        printf("pthread_join error :[%s]", strerror(result));
    }
}

void *threadCancel(void *arg) {
    int num = 0;
    while (1) {
        printf("child pid=[%d] thread id=[%ld] thread num=[%d]\n", getpid(), pthread_self(), num++);
        //设置取消点
        pthread_testcancel();
    }
}

void testCreateThread5() {
    pthread_t thread;
    //创建子线程的时候设置分离属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //设置线程为分离属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&thread, &attr, threadCancel, NULL);

    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }
    printf("main pid=[%d] thread id=[%ld]\n", getpid(), pthread_self());

    //取消子线程
    pthread_cancel(thread);
    //比较线程是否同一进程
    if (pthread_equal(thread, pthread_self())) {

    }
    //sleep(5);//为了让子线程能够 拉起来
    int result = pthread_join(thread, NULL);
    if (result != 0) {
        printf("pthread_join error :[%s]", strerror(result));
    }
    pthread_attr_destroy(&attr);
}
int s_number = 0;
//定义一把锁
pthread_mutex_t  mutex;
void *funThread1(void *arg) {
    int i = 0;
    int n;
    for(i = 0; i < 5000; i++ ){
        //加锁
        pthread_mutex_lock(&mutex);
        n = s_number;
        n++;
        s_number=n;
        //解锁
        pthread_mutex_unlock(&mutex);
    }
}


void *funThread2(void *arg) {
    int i = 0;
    int n;
    for(i = 0; i < 5000; i++ ){
        //加锁
        pthread_mutex_lock(&mutex);
        n = s_number;
        n++;
        s_number=n;
        //解锁
        pthread_mutex_unlock(&mutex);
    }
}


void testCreateThread6() {
    //初始化互斥锁
    pthread_mutex_init(&mutex,NULL);
    pthread_t thread1;
    int ret = pthread_create(&thread1, NULL, funThread1, NULL);
    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }

    pthread_t thread2;
    ret = pthread_create(&thread2, NULL, funThread2, NULL);
    if (ret != 0) {
        printf("pthread_create error,[%s]\n", strerror(ret));
        return;
    }
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    printf("number=[%d]",s_number);
    //释放互斥锁
    pthread_mutex_destroy(&mutex);

}