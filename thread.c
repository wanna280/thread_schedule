#include "thread.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>

static struct thread_struct init_task = {0, NULL, THREAD_STATUS_RUNNING, 0, 0, 15, 15, {0}};

struct thread_struct *current = &init_task; //Init current thread

struct thread_struct *task[THR_TASKS] = {&init_task,}; //Init task list

void schedule();  //Define schedule
void start(struct thread_struct *tsk)
{
    tsk->thread_func();   //Run thread
    tsk->status = THREAD_STATUS_EXIT;  //status
    printf("Thread %d excited\n",tsk->id);
    task[tsk->id] = NULL; //Set current position to NULL
    schedule();           //Schedule
}

int thread_create(int *tid, void (*start_routine)())
{
    int id = -1; //Init the thread_id
    struct thread_struct *tsk = (struct thread_struct *)malloc(sizeof(struct thread_struct));
    //Find a null task position
    while (++id < THR_TASKS && task[id])
        ;

    //Can't find a null task position
    if (id == THR_TASKS)
        return -1;

    task[id] = tsk; //Put the tsk into right position

    if (tid)  //Set id ro tid
        *tid = id;

    tsk->id = id;                              //Set id to thread_id
    tsk->thread_func = start_routine;          //The function of thread processing
    int *stack = tsk->stack;                   // Get address of thread stack
    tsk->esp = (int)(stack + STACK_SIZE - 11); //Get esp-The top of stack
    tsk->status = THREAD_STATUS_RUNNING;       //set running to status
    tsk->wakeuptime = 0;                       //Set wakeuptime
    tsk->counter = 15;                         //Set counter
    tsk->priority = 15;                        //set priority

    // Running stack
    // Low address
    stack[STACK_SIZE - 11] = 7;         // eflags
    stack[STACK_SIZE - 10] = 6;         // eax
    stack[STACK_SIZE - 9] = 5;          // edx
    stack[STACK_SIZE - 8] = 4;          // ecx
    stack[STACK_SIZE - 7] = 3;          // ebx
    stack[STACK_SIZE - 6] = 2;          // esi
    stack[STACK_SIZE - 5] = 1;          // edi
    stack[STACK_SIZE - 4] = 0;          // old ebp
    stack[STACK_SIZE - 3] = (int)start; // ret to start
    stack[STACK_SIZE - 2] = 100;        // ret to unknown
    stack[STACK_SIZE - 1] = (int)tsk;   // The args of start
    // High address
    return 0;
}

int thread_join(int tid)
{
    //While status is not exit,continue to schedule
    while(task[tid]->status != THREAD_STATUS_EXIT)
    {
        schedule();
    }
    free(task[tid]);   //Free memory of task[tid]
    task[tid] = NULL;  //Set this task position to NULL
}
