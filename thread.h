#define STACK_SIZE 1024 //Define max size of thread_stack
#define THR_TASKS 32    //Define max size of thread

#define THREAD_STATUS_RUNNING 0//Status:running
#define THREAD_STATUS_SLEEP 1  //Status:sleep
#define THREAD_STATUS_READY 2  //Status:ready
#define THREAD_STATUS_EXIT 3   //Status:exit


struct thread_struct {
  int id; //thread_id
  void (*thread_func)();//The function for thread processing
  int esp; // esp(Top of running stack)
  unsigned int wakeuptime; // wakeuptime
  int status; // The status of running
  int counter; //The number of counter
  int priority; // The priority value
  int stack[STACK_SIZE];//Running stack
};

int thread_create(int *tid,void (*start_routine)());
int thread_join(int tid);
void mysleep(int seconds);
