#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "ls_new.c"

int main(void){
  create_container(1);
  create_container(2);
  create_container(3);
  // Three container managers ( user programs ) should be running now */
  // ==========================================================
  /* Testing of resource isolation */

  int pid1 = fork();
  if (pid1 == 0){
    join_container(1); //called only by child created by preceeding fork call .
    int fd = open("myfile", O_CREATE | O_RDWR);
    write(fd,"modified by cont 1\n", 20);
    close(fd);
    // ls();
    leave_container();
    exit();
    ps();
  }

  int pid2 = fork();
  // printf(1, "pid2 %d\n", pid2);
  if (pid2 == 0){
    join_container(2); //called only by child created by preceeding fork call .
    int fd = open("myfile", O_CREATE | O_RDWR);
    write(fd,"modified by cont 2\n", 20);
    close(fd);
    leave_container();
    exit();
    ps();
  }

  join_container(3); // called only by child created by preceeding fork call .
  /* Testing of resource isolation */
  int fd = open("myfile", O_CREATE | O_RDWR);
  write(fd,"modified by cont -1\n", 21);
  ls_new(".", getcid());
  close(fd);
  /* ---------- PROCESS ISOLATION ------------------*/
  // called by atmost one process in every container .
  ps();

  wait();
  wait();
  exit();
  return 0;
}
