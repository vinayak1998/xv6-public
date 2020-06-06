#include <sys/stat.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdio.h>


void critical_section(int type, int pid){
  if(type == 1){
    return;
  }
  else if (type == 2){
    printf("%d acquired the lock at time %ld\n",pid,time(NULL));
    sleep(2);
    printf("%d released the lock at time %ld\n",pid,time(NULL));
    return;
  }
  else{
    printf("%d acquired the lock at time %ld\n",pid,time(NULL));
    printf("%d released the lock at time %ld\n",pid,time(NULL));
    return;
  }
}

void debug1(int a){
  if (a==0){
    return;
  }
  else{
    printf("debug statement %d\n", a);
    return;
  }
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}


void set_parameters(void){
  int temp = 00;
  for(int i = 0; i<11; i++){
    temp+=1;
  }
  return;
}

void fix_pipe(void){
  int temp = 00;
  for(int i = 0; i<11; i++){
    temp+=1;
  }
  return;
}

int main(int argc, char *argv[]){
  int temp[4];
  FILE *fd = fopen(argv[1], "r");
  fscanf(fd, "%d", &temp[0]);
  fscanf(fd, "%d", &temp[1]);
  fscanf(fd, "%d", &temp[2]);
  fscanf(fd, "%d", &temp[3]);
  fclose(fd);
  // int P, P1, P2, P3;
  int P = *(temp + 0);
  int P1 = *(temp + 1);
  int P2 = *(temp + 2);
  int P3 = *(temp + 3);

  int pipe_reqs[P][2];
  int pipe_par[2];
  int pipe_child[P][2];
  int pipe_extra[P][2];
  set_parameters();
  for(int i = 0; i < P; i++){
    fix_pipe();
    pipe(pipe_child[i]);
    pipe(pipe_reqs[i]);
    pipe(pipe_extra[i]);
  }
  pipe(pipe_par);
  int total_requests = P - P1;
  int i, j, k, z;
  int ids, type;
  set_parameters();
  k = 0;
  while(k < P){
    if(fork()== 0){
      debug1(0);
      char msg, ret, rels;
      int m = k;
      int id = 0;
      int num_grants = 0;
      debug1(0);
      if(k >= P1){
        z = 0;
        while(z < P){
          fix_pipe();
          write(pipe_reqs[z][1], (char*)&m, sizeof(int));
          z++;
        }
      }
      fix_pipe();
      write(pipe_par[1], (char*)&m, sizeof(int));
      int requests[total_requests];
      // debug1(0);
      read(pipe_child[k][0], (char*)&m, sizeof(int));
      i = 0;
      while(i < total_requests){
        read(pipe_reqs[k][0], (char*)&requests[i], sizeof(int));
        i++;
      }
      debug1(0);
      mergeSort(requests, 0,total_requests);

      debug1(0);
      i = 0;
      while(i < total_requests){
        msg = 'G';
        // msg_id = m;
        fix_pipe();
        write(pipe_extra[requests[i]][1], (char*)&msg, sizeof(char));

        while(1){
          debug1(0);
          read(pipe_extra[k][0], (char*)&ret, sizeof(char));
          if(ret == 'G'){
            num_grants = num_grants+1;
            debug1(0);
            if(num_grants == P){
              type = 2;
              debug1(0);
              if(k >= (P - P3)){
                type = 3;
              }
              //int pid = getpid();
              critical_section(type, getpid());
              debug1(0);
              rels = 'R';
              j = 0;
              while(j<P){
                fix_pipe();
                write(pipe_extra[j][1], (char*)&rels, sizeof(char));
                j++;
              }
              num_grants = 0;
            }
          }
          else if(ret == 'R'){
            break;
          }
        }
        i++;
      }
      exit(0);
    }
    k++;
  }
  ids = 0;
  debug1(0);
  k = 0;
  while(k<P){
    read(pipe_par[0], (char*)&ids, sizeof(int));
    debug1(0);
    write(pipe_child[ids][1], (char*)&ids, sizeof(int));
    k++;
  }

  k = 0;
  while (k<P) {
    debug1(0);
    wait(0);
    k++;
  }
  exit(0);
}
