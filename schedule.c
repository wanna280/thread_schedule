#include "thread.h"
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

extern struct thread_struct *current;         //Has defined in schedule.c
extern struct thread_struct *task[THR_TASKS]; //Has defined in schedule.c

//Declare function switch_to_next
//function switch_to_next has defined in switch.s
void switch_to_next(struct thread_struct *next);

static unsigned int getmstime() //Function getmstime
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
    {
        perror("gettimeofday");
        exit(-1);
    }
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//Function pick in order to select next thread to run
//It was written by Linus,by using algorithm RR
//I think that is the best way to shedule
static struct thread_struct *pick()
{
    int i, next, c;

    for (i = 0; i < THR_TASKS; ++i)
    {
        if (task[i] && task[i]->status != THREAD_STATUS_EXIT && getmstime() > task[i]->wakeuptime)
        {
            task[i]->status = THREAD_STATUS_RUNNING;
        }
    }

    while (1)
    {
        c = -1;
        next = 0;
        for (i = 0; i < THR_TASKS; ++i)
        {
            if (!task[i])
                continue;
            if (task[i]->status == THREAD_STATUS_RUNNING && task[i]->counter > c)
            {
                c = task[i]->counter;
                next = i;
            }
        }
        if (c)
            break;

        // If all counter are zero,reset the counter
        if (c == 0)
        {
            for (i = 0; i < THR_TASKS; ++i)
            {
                if (task[i])
                {
                    task[i]->counter = task[i]->priority + (task[i]->counter >> 1);
                }
            }
        }
    }

    return task[next]; //Return the next struct of thread
}

//Close alarm
void closealarm()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
    {
        perror("sigprocmask BLOCK");
    }
}

//Open alarm
void openalarm()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) < 0)
    {
        perror("sigprocmask BLOCK");
    }
}

//Fuction schedule
//In order to switch to next task
void schedule()
{
    struct thread_struct *next = pick();
    if (next)
    {
        switch_to_next(next);
    }
    // schedule never return
}

//Function mysleep in oreder to achieve sleep
void mysleep(int seconds)
{ //pass the arg seconds

    //Set the wakeup time of current thread
    current->wakeuptime = getmstime() + 1000 * seconds;

    //Set the status of current thread to sleep
    current->status = THREAD_STATUS_SLEEP;

    //sleep and schedule to run other threads
    schedule();
}

static void do_timer()
{ //Function Clock interrupt
    if (--current->counter > 0)
        return;
    current->counter = 0;
    schedule();
}

__attribute__((constructor)) //Clock interrupt
static void
init()
{
    struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1000;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = 1000 * 10; // vulue is 10 ms
    if (setitimer(ITIMER_REAL, &value, NULL) < 0)
    {
        perror("setitimer");
    }
    signal(SIGALRM, do_timer);
}
