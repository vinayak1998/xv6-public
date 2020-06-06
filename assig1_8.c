#include "types.h"
#include "stat.h"
#include "user.h"
#define MSGSIZE 8


int array_sum(short *arr, int a, int b){
	int temp = 0;
	for(int i = a;i<b;i++){
		temp+=arr[i];
	}
	return temp;
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}


int
main(int argc, char *argv[])
{
	if(argc< 2){
		printf(1,"Need type and input filename\n");
		exit();
	}
	char *filename;
	filename=argv[2];
	int type = atoi(argv[1]);
	printf(1,"Type is %d and filename is %s\n", type, filename);

	int tot_sum = 0;
	float variance = 0.0;

	int size=1000;
	short arr[size];
	char c;
	int fd = open(filename, 0);
	for(int i=0; i<size; i++){
		read(fd, &c, 1);
		arr[i]=c-'0';
		read(fd, &c, 1);
	}
  	close(fd);
  	// this is to supress warning
  	printf(1,"first elem %d\n", arr[0]);

  	//----FILL THE CODE HERE for unicast sum and multicast variance
		int parent = getpid();
		for(int i  = 0; i<8; i++){
			int child = fork();
			if (child !=0){
				wait();
			}
			else{
				int par_sum = 0;
				par_sum = array_sum(arr, i*(size/8), (i+1)*(size/8));
				char *sum_message = (char *)malloc(MSGSIZE);
				// sum_message = (char *)par_sum;
				tostring(sum_message, par_sum);
			  // sprintf(sum_message, "%f", par_sum);
				send(getpid(), parent, sum_message);
				free(sum_message);
				exit();
			}
		}
		for(int i = 0; i<8;i++){
			char *recv_message = (char *)malloc(MSGSIZE);
			recv(recv_message);
			tot_sum += atoi(recv_message);
		}

  	//------------------
		float mean,sum1;
		sum1 = 0.0;
		mean = tot_sum/(float)size;
		for (int i = 0; i < size; i++)
		{
			sum1 = sum1 + (arr[i] - mean)*(arr[i] - mean);
		}
		variance = sum1/size;
		//
  	if(type==0){ //unicast sum
		printf(1,"Sum of array for file %s is %d\n", filename,tot_sum);
	}
	else{ //mulicast variance
		printf(1,"Variance of array for file %s is %d\n", filename,(int)variance);
	}
	exit();
}
