#include <sys/stat.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdio.h>


float stof(char* s){
  float rez = 0, fact = 1;
  if (*s == '-'){
    s++;
    fact = -1;
  };
  for (int point_seen = 0; *s; s++){
    if (*s == '.'){
      point_seen = 1;
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <= 9){
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    };
  };
  return rez * fact;
};

int pow(int a, int b){
	int temp = 1;
	for (int i = 0; i<b; i++){
		temp *= a;
	}
	return temp;
}

// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
	 int i=0, j=len-1, temp;
	 while (i<j)
	 {
			 temp = str[i];
			 str[i] = str[j];
			 str[j] = temp;
			 i++; j--;
	 }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
int ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10,afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
		return i;
}

// void tostring(char str[], float num, int p, int m)
// {
//     int i, rem, len = 0, n;
//
//     n = num;
//     while (n != 0)
//     {
//         len++;
//         n /= 10;
//     }
//     for (i = 0; i < len; i++)
//     {
//         rem = num % 10;
//         num = num / 10;
//         str[len - (i + 1)] = rem + '0';
//     }
//     str[len] = '\0';
// }



float fabsm(float a){
	if(a<0)
	return -1*a;
return a;
}
int main(int argc, char *argv[])
{
	if(argc< 2){
		printf("Need filename\n");
		exit(0);
	}

  float temp[5];
  FILE *fd = fopen(argv[1], "r");
  fscanf(fd, "%f", &temp[0]);
  fscanf(fd, "%f", &temp[1]);
  fscanf(fd, "%f", &temp[2]);
  fscanf(fd, "%f", &temp[3]);
  fscanf(fd, "%f", &temp[4]);
  fclose(fd);

  int N = (int)*(temp + 0);
  float E = *(temp + 1);
  float T = *(temp + 2);
  int P = (int)*(temp + 3);
  int L = (int)*(temp + 4);
  if (P == 1){
	P = 2;
  }

	// float diff;
	int i,j;
	float mean;
	float u[N][N];
	float w[N][N];

	// int count=0;
	mean = 0.0;
	for (i = 0; i < N; i++){
		u[i][0] = u[i][N-1] = u[0][i] = T;
		u[N-1][i] = 0.0;
		mean += u[i][0] + u[i][N-1] + u[0][i] + u[N-1][i];
	}
	mean /= (4.0 * N);
	// printf("%d ",((int)mean));
	for (i = 1; i < N-1; i++ ){
		for ( j= 1; j < N-1; j++){
			u[i][j] = mean;
		}
	}
	// for(i =0; i <N; i++){
	// 	for(j = 0; j<N; j++){
	// 		printf("%d ",((int)u[i][j]));
	// 	}
	// 	printf("\n");
	// }
	//


	//-----------------------------------------------------------------------------
	int m = (N-2)/P;
	int m_last = (N-2) - (m)*(P-1);
	// int child_arr[P];
	// printf("N : %d\n",N);
	// printf("P : %d\n",P);
	// printf("m : %d\n",m);
	// printf("m_last %d\n",m_last);
	int parent = getpid();
	// printf("parent : %d\n",parent);
	int msg_size = (int) N*T*m;
	int count = 0;
	float diff;
  diff = T;
  int pipe_data_to_parent[P][2];
  for (int i = 0; i<P; i++){
    pipe(pipe_data_to_parent[i]);
  }
	int lol;
	for(;;){
		for(int p = 0; p < P; p++){
			int child = fork();
			if (child !=0){
				wait(0);
			}
			else{
				char *message_to_par = (char *)malloc(sizeof(char)*msg_size);
				char res[20];
				int msg_count = 0;
				if(p == 0){
					*(message_to_par + msg_count) = '0';
					msg_count++;
					*(message_to_par + msg_count) = '-';
					msg_count++;
					for (i = 1; i < m+1; i++){
						for (j = 1; j < N-1; j++){
							w[i][j] = 0.25*(u[i-1][j]+u[(i+1)][j]+u[i][j-1]+u[i][j+1]);
							lol = ftoa(w[i][j], res, 5);
							for(int k = 0; k<lol; k++){
								*(message_to_par + msg_count) = *(res + k);
								msg_count++;
							}
							for(int k = 0; k<5; k++){
								*(message_to_par + msg_count) = *(res + lol + k);
								msg_count++;
							}
							*(message_to_par + msg_count) = '-';
							msg_count++;
						}
					}
					*(message_to_par + msg_count) = '!';
					// printf( "temp-------------------------%s\n", message_to_par);
					write(pipe_data_to_parent[p][1], message_to_par, sizeof(char)*msg_size);
					// tostring(message_to_par, w, p, m);
					// send(getpid(), parent, message_to_par);
					// printf( "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf("%d ",((int)w[i][j]));
					// 	}
					// 	printf( "\n");
					// }
					exit(0);
				}
				else if(p == P-1){
					*(message_to_par + msg_count) = p + '0';
					msg_count++;
					*(message_to_par + msg_count) = '-';
					msg_count++;
					for (i = m*p+1; i < m*p + 1 + m_last; i++){
						for (j = 1; j < N-1; j++){
							w[i][j] = 0.25*(u[i-1][j]+u[(i+1)][j]+u[i][j-1]+u[i][j+1]);
							lol = ftoa(w[i][j], res, 5);
							for(int k = 0; k<lol; k++){
								*(message_to_par + msg_count) = *(res + k);
								msg_count++;
							}
							for(int k = 0; k<5; k++){
								*(message_to_par + msg_count) = *(res + lol + k);
								msg_count++;
							}
							*(message_to_par + msg_count) = '-';
							msg_count++;
						}
					}
					*(message_to_par + msg_count) = '!';
					// printf( "temp-------------------------%s\n", message_to_par);
					write(pipe_data_to_parent[p][1], message_to_par, sizeof(char)*msg_size);
					// tostring(message_to_par, w, p);
					// send(getpid(), parent, message_to_par);
					// printf( "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf("%d ",((int)w[i][j]));
					// 	}
					// 	printf( "\n");
					// }
					exit(0);
				}
				else{
					*(message_to_par + msg_count) = p + '0';
					msg_count++;
					*(message_to_par + msg_count) = '-';
					msg_count++;
					for (i = m*p + 1; i < m*p + 1 + m; i++){
						for (j = 1; j < N-1; j++){
							w[i][j] = 0.25*(u[i-1][j]+u[(i+1)][j]+u[i][j-1]+u[i][j+1]);
							lol = ftoa(w[i][j], res, 5);
							for(int k = 0; k<lol; k++){
								*(message_to_par + msg_count) = *(res + k);
								msg_count++;
							}
							for(int k = 0; k<5; k++){
								*(message_to_par + msg_count) = *(res + lol + k);
								msg_count++;
							}
							*(message_to_par + msg_count) = '-';
							msg_count++;
						}
					}
					*(message_to_par + msg_count) = '!';
					// printf( "temp-------------------------%s\n", message_to_par);
					write(pipe_data_to_parent[p][1], message_to_par, sizeof(char)*msg_size);
					// printf( "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf("%d ",((int)w[i][j]));
					// 	}
					// 	printf( "\n");
					// }
					exit(0);
				}
			}
		}
		for(int i = 0; i<P;i++){
				char *recv_message = (char *)malloc(sizeof(char)*msg_size);
				read(pipe_data_to_parent[i][0], recv_message, sizeof(char)*msg_size);
				char *num = (char *)malloc(sizeof(char)*15);
				int p = *recv_message - '0';
				// printf( "value of %d\n", p);
				// printf( "%s\n", recv_message);
				float number;
				int flag = 0;
				int counter = 1;
				int counter1 = 0, num_counter = 0, num_row = 0, flag1 = 0;
				while(flag == 0){
					if (*(recv_message + counter) == '-'){
						num = (char *)malloc(sizeof(char)*10);
						flag1 = 0;
						counter1 = 0;
						// printf( "here - \n");
					}
					// printf( "debug1\n");
					else if (*(recv_message + counter) == '!'){
						flag = 1;
						// printf( "here ! \n");
					}
					else{
							// printf( "here num\n");
							while(flag1 == 0){
								if (*(recv_message + counter) == '-'){
									flag1 = 1;
									counter--;
									// printf( "here 2nd - \n");
								}
								else{
									*(num + counter1) = *(recv_message + counter);
									counter1++;
									counter++;
									// printf( "here counter %d \n", counter);
								}
							}
							// printf( "here\n");
							number = stof(num);
							// printf( "here1\n");
							// printf( "recieved %d\n", (int) number);
							if((num_counter%(N-2)) == 0){
								num_row++;
							}
							w[num_row+m*p][1+(num_counter%(N-2))] = number;
							num_counter ++;
					}
					counter++;
				}
				// printf( "%d\n", num_counter);
			}

		count++;

    float max_diff = -1.0;
    float this_diff = 0.0;
    for(i = 1; i < N-1; i++){
			for(j = 1; j<N-1; j++){
				this_diff = fabsm(u[i][j]-w[i][j]);
        if (this_diff > max_diff){
          max_diff = this_diff;
        }
        u[i][j] = w[i][j];
			}
    }

    if (max_diff<diff){
      diff = max_diff;
    }
    // char ttt[20];
    // int llll = ftoa(diff, ttt, 7);
    // printf( "dump %d\n", llll);
    //
    // printf( "diff %s\n", ttt);
    // printf(1 ,"count %d\n", count);
    if(diff<= E || count > L){
      break;
    }
	}

  for(i = 0; i < N; i++){
    	for(j = 0; j<N; j++){
      		printf("%d ",((int)u[i][j]));
      	}
      	printf("\n");
      }
  exit(0);
}
