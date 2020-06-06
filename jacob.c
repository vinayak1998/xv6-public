#include "types.h"
#include "stat.h"
#include "user.h"

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
		printf(1,"Need filename\n");
		exit();
	}
	char *filename;
	filename=argv[1];
	// printf(1,"Filename is %s\n", filename);


	int size=5;
	int flag, counter;
	int temp_arr[size];
	char temp;
	int fd = open(filename, 0);
	for(int i=0; i<size; i++){
		flag = 0;
		counter = 0;
		while(flag == 0){
			read(fd, &temp, 1);
			if ((int)temp == 10){
				flag = 1;
			}
			else{
				counter++;
			}
		}
		temp_arr[i] = counter;
	}
		close(fd);
	// printf(1,"first elem length %d\n", temp_arr[0]);
	// printf(1,"second elem length %d\n", temp_arr[1]);
	// printf(1,"third elem length %d\n", temp_arr[2]);
	// printf(1,"fourth elem length%d\n", temp_arr[3]);
	// printf(1,"fifth elem length%d\n", temp_arr[4]);

	// int abc;
	char* arr[size];
	fd = open(filename, 0);
	char* dump;
	dump = (char *)malloc(sizeof(char));
	for(int i=0; i<size; i++){
		char *c;
		c = (char *)malloc(sizeof(char)*20);
		read(fd, c, temp_arr[i]);
		// printf(1,"%s \n", c);
		// printf(1,"%d \n", abc);
		arr[i] = c;
		read(fd, dump, 1);
	}
  	close(fd);
	// this is to supress warning
	// printf(1,"first elem %s\n", arr[0]);
	// printf(1,"second elem %s\n", arr[1]);
	// printf(1,"third elem %s\n", arr[2]);
	// printf(1,"fourth elem %s\n", arr[3]);
	// printf(1,"fifth elem %s\n", arr[4]);

		float final_val[5];
		for (int i=0;i<5;i++){
			final_val[i] = stof(arr[i]);
		}
		// printf(1,"first elem %d\n", final_val[0]);
		// printf(1,"second elem %d\n", final_val[1]);
		// printf(1,"third elem %d\n", final_val[2]);
		// printf(1,"fourth elem %d\n", final_val[3]);
		// printf(1,"fifth elem %d\n", final_val[4]);

	int N = (int)final_val[0];
	float E = final_val[1];
	float T = final_val[2];
	int P = (int)final_val[3];
	int L = (int)final_val[4];
	if (P==1){
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
	// printf(1,"%d ",((int)mean));
	for (i = 1; i < N-1; i++ ){
		for ( j= 1; j < N-1; j++){
			u[i][j] = mean;
		}
	}
	// for(i =0; i <N; i++){
	// 	for(j = 0; j<N; j++){
	// 		printf(1,"%d ",((int)u[i][j]));
	// 	}
	// 	printf(1,"\n");
	// }
	//


	//-----------------------------------------------------------------------------
	int m = (N-2)/P;
	int m_last = (N-2) - (m)*(P-1);
	// int child_arr[P];
	// printf(1,"N : %d\n",N);
	// printf(1,"P : %d\n",P);
	// printf(1,"m : %d\n",m);
	// printf(1,"m_last %d\n",m_last);
	int parent = getpid();
	// printf(1,"parent : %d\n",parent);
	int msg_size = (int) N*T*m;
	int count = 0;
	float diff;
  diff = T;

	int lol;
	for(;;){
		for(int p = 0; p < P; p++){
			int child = fork();
			if (child !=0){
				wait();
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
					// printf(1, "temp-------------------------%s\n", message_to_par);
					send(getpid(), parent, message_to_par);
					// tostring(message_to_par, w, p, m);
					// send(getpid(), parent, message_to_par);
					// printf(1, "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf(1,"%d ",((int)w[i][j]));
					// 	}
					// 	printf(1, "\n");
					// }
					exit();
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
					// printf(1, "temp-------------------------%s\n", message_to_par);
					send(getpid(), parent, message_to_par);
					// tostring(message_to_par, w, p);
					// send(getpid(), parent, message_to_par);
					// printf(1, "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf(1,"%d ",((int)w[i][j]));
					// 	}
					// 	printf(1, "\n");
					// }
					exit();
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
					// printf(1, "temp-------------------------%s\n", message_to_par);
					send(getpid(), parent, message_to_par);
					// printf(1, "child id %d\n", getpid());
					// for(i = 0; i < N; i++){
					// 	for(j = 0; j<N; j++){
					// 		printf(1,"%d ",((int)w[i][j]));
					// 	}
					// 	printf(1, "\n");
					// }
					exit();
				}
			}
		}
		for(int i = 0; i<P;i++){
				char *recv_message = (char *)malloc(sizeof(char)*msg_size);
				recv(recv_message);
				char *num = (char *)malloc(sizeof(char)*15);
				int p = *recv_message - '0';
				// printf(1, "value of %d\n", p);
				// printf(1, "%s\n", recv_message);
				float number;
				int flag = 0;
				int counter = 1;
				int counter1 = 0, num_counter = 0, num_row = 0, flag1 = 0;
				while(flag == 0){
					if (*(recv_message + counter) == '-'){
						num = (char *)malloc(sizeof(char)*10);
						flag1 = 0;
						counter1 = 0;
						// printf(1, "here - \n");
					}
					// printf(1, "debug1\n");
					else if (*(recv_message + counter) == '!'){
						flag = 1;
						// printf(1, "here ! \n");
					}
					else{
							// printf(1, "here num\n");
							while(flag1 == 0){
								if (*(recv_message + counter) == '-'){
									flag1 = 1;
									counter--;
									// printf(1, "here 2nd - \n");
								}
								else{
									*(num + counter1) = *(recv_message + counter);
									counter1++;
									counter++;
									// printf(1, "here counter %d \n", counter);
								}
							}
							// printf(1, "here\n");
							number = stof(num);
							// printf(1, "here1\n");
							// printf(1, "recieved %d\n", (int) number);
							if((num_counter%(N-2)) == 0){
								num_row++;
							}
							w[num_row+m*p][1+(num_counter%(N-2))] = number;
							num_counter ++;
					}
					counter++;
				}
				// printf(1, "%d\n", num_counter);
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
    // printf(1, "dump %d\n", llll);
    //
    // printf(1, "diff %s\n", ttt);
    // printf(1 ,"count %d\n", count);
    if(diff<= E || count > L){
      break;
    }
	}

  for(i = 0; i < N; i++){
    	for(j = 0; j<N; j++){
      		printf(1,"%d ",((int)u[i][j]));
      	}
      	printf(1,"\n");
      }
  exit();
}
