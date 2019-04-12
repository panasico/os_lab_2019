#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include <stddef.h>


pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut5 = PTHREAD_MUTEX_INITIALIZER;

void mutex_1(ptrdiff_t adr){
    pthread_mutex_lock(&mut1);
    printf("\npthread %p mutex_locking int mutex_1()\n", adr);
    printf("try launch mutex_2()...\n", adr);
    mutex_2(adr);
}

void mutex_2(ptrdiff_t adr){
    pthread_mutex_lock(&mut2);
    printf("\npthread %p mutex_locking int mutex_2()\n", adr);
    printf("try launch mutex_3()...\n", adr);
    mutex_3(adr);
}
void mutex_3(ptrdiff_t adr){
    pthread_mutex_lock(&mut3);
    printf("\npthread %p mutex_locking int mutex_3()\n", adr);
    printf("try launch mutex_4()...\n", adr);
    mutex_4(adr);
}
void mutex_4(ptrdiff_t adr){
    pthread_mutex_lock(&mut4);
    printf("\npthread %p mutex_locking int mutex_4()\n", adr);
    printf("try launch mutex_5()...\n", adr);
    mutex_5(adr);
}
void mutex_5(ptrdiff_t adr){
    pthread_mutex_lock(&mut5);
    printf("\npthread %p mutex_locking int mutex_5()\n", adr);
    printf("try launch mutex_1()...\n", adr);
    mutex_1(adr);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;
    
    printf("thread 1 is %p\n", &thread1);
    printf("thread 2 is %p\n\n", &thread2);
    printf("thread 3 is %p\n\n", &thread3);
    printf("thread 4 is %p\n\n", &thread4);
    printf("thread 5 is %p\n\n", &thread5);
    
    pthread_create(&thread1, NULL, mutex_1, (void *)&thread1);
    pthread_create(&thread2, NULL, mutex_2, (void *)&thread2);
    pthread_create(&thread3, NULL, mutex_3, (void *)&thread3);
    pthread_create(&thread4, NULL, mutex_4, (void *)&thread4);
    pthread_create(&thread5, NULL, mutex_5, (void *)&thread5);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    pthread_join(thread5,NULL);
    
    printf("hello, world\n");
}