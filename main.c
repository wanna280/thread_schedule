#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "func.h"

extern void schedule();

int main()
{

    int tid1, tid2, tid3, tid4;
    thread_create(&tid1, fun1);
    printf("Thread %d has been created\n", tid1);
    thread_create(&tid2, fun2);
    printf("Thread %d has been created\n", tid2);
    thread_create(&tid3, fun3);
    printf("Thread %d has been created\n", tid3);
    thread_create(&tid4, fun4);
    printf("Thread %d has been created\n", tid4);


    int i = 2;
    while (i--)
    {
        printf("Wanna--Main is running\n");
        mysleep(3);
    }
    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);
    thread_join(tid4);
    
    return 0; 
}
