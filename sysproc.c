#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#define NUM_SYS 37


//
int toggle_val = 0;
int print_values;
extern int count_sys[NUM_SYS];
extern void print_proc(void);
extern int send();
extern int recv();
extern int send_multi();
extern int scheduler_log;
//



int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


//appends

//shutdown
int
sys_halt(void)
{
  outb(0xf4,0x00);
  return 0;
}


//sys_count function
int
sys_print_count(void)
{
  if (toggle_val){
    print_values = 1;
  }
  else{
    print_values = 0;
  }
  return 0;
}

//toggle function
int
sys_toggle(void)
{
  toggle_val = 1 - toggle_val;
  for(int i = 0; i<NUM_SYS; i++){
    count_sys[i] = 0;
  }
  return 0;
}


//add function
int
sys_add(void)
{
  int a,b;
  argint(0,&a);
  argint(1,&b);
  return a+b;

}

// printing all processes
int
sys_ps(void)
{
  print_proc();
  return 0;
}

// unicast send
int
sys_send(void)
{
  int a,b;
  char* msg;
  argint(0,&a);
  argint(1,&b);
  argstr(2,&msg);
  return send(a, b, (void*) msg);
}

//receive with blocking
int
sys_recv(void)
{
  char* msg;
  argstr(0,&msg);
  return recv((void*)msg);
}

//multicast send
int
sys_send_multi(void)
{
  int a,b,c;
  char* msg;
  argint(0,&a);
  argint(1,&b);
  argint(2,&c);
  argstr(3,&msg);
  return send_multi(a,b,(void*)msg,c);
}

int
sys_create_container(void)
{
  int cid;
  (void)argint(0, &cid);
  return call_create_container(cid);
}

int
sys_destroy_container(void)
{
  int cid;
  (void)argint(0, &cid);
  return call_destroy_container(cid);
}

int
sys_join_container(void)
{
  int cid;
  (void)argint(0, &cid);
  return call_join_container(cid);
}

int
sys_leave_container(void)
{
  return call_leave_container();
}

int
sys_scheduler_log_on(void)
{
  scheduler_log = 1;
  return 1;
}

int
sys_scheduler_log_off(void)
{
  scheduler_log = 0;
  return 1;
}

int
sys_getcid(void)
{
  return myproc()->cid;
}
