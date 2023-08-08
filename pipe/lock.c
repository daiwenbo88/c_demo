//
// Created by 代文波 on 2023/7/11.
//
#include <pthread.h>
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include <sys/stat.h>
#include "unistd.h"
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/semaphore.h>
#include "lock.h"
#include "linkList.h"

int n_number = 0;
//定义一把锁
pthread_rwlock_t rwlock;

//写线程
void *thread_write(void *arg) {
    int cur;
    while (1) {
        //加写锁
        pthread_rwlock_wrlock(&rwlock);
        cur = n_number;
        cur++;
        n_number = cur;
        //解锁
        printf("write number=[%d]\n", cur);
        pthread_rwlock_unlock(&rwlock);
        sleep(rand() % 3);
    }
}

//读线程
void *thread_read(void *arg) {
    int cur;
    while (1) {
        //添加读锁
        pthread_rwlock_rdlock(&rwlock);
        cur = n_number;
        printf("read number=[%d]\n", cur);
        //解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(rand() % 3);
    }
}

void testRwlock() {
    pthread_t thread[8];
    //初始化锁
    pthread_rwlock_init(&rwlock, NULL);

    int i = 0;
    for (; i < 3; i++) {
        int ret = pthread_create(&thread[i], NULL, thread_write, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }
    }
    for (; i < 5; i++) {
        int ret = pthread_create(&thread[3 + i], NULL, thread_read, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }
    }
    int j = 0;
    for (j = 0; j < 8; j++) {
        pthread_join(thread[j], NULL);
    }
    //释放锁
    pthread_rwlock_destroy(&rwlock);
}

//定义一把锁
pthread_mutex_t s_mutex;
linkList list;
pthread_cond_t cond;

//生产者
void *producer(void *arg) {
    while (1) {
        //加锁
        pthread_mutex_lock(&s_mutex);
        list_tail_insert(list, 10);
        //解锁
        pthread_mutex_unlock(&s_mutex);
        //唤醒至少一个阻塞在该条件变量上的线程
        pthread_cond_signal(&cond);
        printf("producer insert value success\n");
        list_show(list);
        sleep(1);
    }
}


void *consumer(void *arg) {
    printf("consumer poo\n");
    while (1) {
        //加锁
        pthread_mutex_lock(&s_mutex);
        if (listEmpty(list) == 0) {
            printf("consumer cond wait\n");
            //条件不满足, 引起线程阻塞并解锁;
            //条件满足, 解除线程阻塞, 并加锁
            // cond 条件变量, mutex 互斥锁变量
            pthread_cond_wait(&cond, &s_mutex);
        }
        linkList value = list_poo(list);
        //解锁
        pthread_mutex_unlock(&s_mutex);
        if (NULL!=value){
            printf("consumer value = [%d]\n", value->data);
            free(value);
            value = NULL;
        } else{
            continue;
        }
        sleep(1);
    }
}


void testThreadMutex() {
    srand(time(NULL));
    list = listCreate();
    //初始化互斥锁
    pthread_mutex_init(&s_mutex, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);
    pthread_t thread1[5];
    pthread_t thread2[5];
    for (int i = 0; i < 5; i++) {
        int ret = pthread_create(&thread1[i], NULL, producer, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }

        ret = pthread_create(&thread2[i], NULL, consumer, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(thread1[i], NULL);
        pthread_join(thread2[i], NULL);
    }
    //释放互斥锁
    pthread_mutex_destroy(&s_mutex);
    //销毁条件变量
    pthread_cond_destroy(&cond);

}
sem_t sem_producer;
sem_t sem_consumer;

//生产者
void *producer2(void *arg) {
    while (1) {
        //加锁
        sem_wait(&sem_producer);
        list_tail_insert(list, 10);
        //解锁
        sem_post(&sem_consumer);
        printf("producer insert value success\n");
        list_show(list);
        sleep(1);
    }
}


void *consumer2(void *arg) {
    printf("consumer poo\n");
    while (1) {
        //加锁
        sem_wait(&sem_consumer);
        linkList value = list_poo(list);
        if (NULL!=value){
            printf("consumer value = [%d]\n", value->data);
            free(value);
            value = NULL;
        }
        //解锁
        sem_post(&sem_producer);
        sleep(1);
    }
}


void testThreadMutex2() {
    srand(time(NULL));
    list = listCreate();
    //初始化信号量
    sem_init(&sem_producer, 0, 5);
    sem_init(&sem_consumer, 0, 5);
    pthread_t thread1[5];
    pthread_t thread2[5];
    for (int i = 0; i < 5; i++) {
        int ret = pthread_create(&thread1[i], NULL, producer2, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }

        ret = pthread_create(&thread2[i], NULL, consumer2, NULL);
        if (ret != 0) {
            printf("pthread_create error,[%s]\n", strerror(ret));
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(thread1[i], NULL);
        pthread_join(thread2[i], NULL);
    }
    sem_destroy(&sem_consumer);
    sem_destroy(&sem_producer);

}