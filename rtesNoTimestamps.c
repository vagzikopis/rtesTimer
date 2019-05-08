						
//			RTES-ASSIGNEMENT1		  //
// Author Zikopis Evangelos AEM: 8808 //


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>
#include <fcntl.h> 
#include <math.h>

int alarm_usec;
int flag = 0;
int timestamp_sec;
int timestamp_usec;
struct timeval tv;

static void ualarm_handler(int sig){
	gettimeofday(&tv, NULL);
	timestamp_sec = tv.tv_sec;
	timestamp_usec = tv.tv_usec;
	flag = 1;
}

static void alarm_handler(int sig){
	signal(SIGALRM, alarm_handler);
	if (alarm_usec > 0){
		signal(SIGALRM, ualarm_handler);
		ualarm(alarm_usec, 0);
	}else{
		gettimeofday(&tv, NULL);
		timestamp_sec = tv.tv_sec;
		timestamp_usec = tv.tv_usec;
		flag = 1;
	}
}


int main(){

	float alarm_time, temp;
	int  size, total_time, alarm_sec;
	// uncomment filename variable and enter your path to save data //
	/*char filename[] = "data/final_timestamps";					
	int fd;*/
	printf("Enter total sampling time: ");
	scanf("%d", &total_time);
	printf("Enter interval time: ");
	scanf("%f", &alarm_time);

	if (alarm_time >= 1){
		alarm_sec = (int)(alarm_time);
		alarm_usec = (int)((alarm_time - alarm_sec)*1000000);
		signal(SIGALRM, alarm_handler);
	}else{
		alarm_usec = (int)((alarm_time)*1000000);
		signal(SIGALRM, ualarm_handler);
	}

	size = (int)(total_time / alarm_time)+1;
	printf("size:%d\n", size); 
	struct timeval table[size+1];

	gettimeofday(&tv, NULL);
	table[0].tv_sec = tv.tv_sec;
	table[0].tv_usec = tv.tv_usec;
	int i = 1;
	if (!alarm_sec){
		ualarm(alarm_usec,0);
	}else{
		alarm(alarm_sec);
	}
	while(i<(size)){
		while(flag){
			table[i].tv_sec = timestamp_sec;
			table[i].tv_usec = timestamp_usec;
			i++;
			if (!alarm_sec){
				ualarm(alarm_usec,0);
				signal(SIGALRM, ualarm_handler);
			}else{
				alarm(alarm_sec);
				signal(SIGALRM, alarm_handler);
			}
			flag = 0;
		}
	}

// Optional step, used to save data to a txt file //
/*	fd = creat(filename, 0644);	 
	dup2(fd, STDOUT_FILENO);
	close(fd);*/ 
	printf("Total Sampling Time: %d\n", total_time);
	printf("Interval Time: %f usec\n\n", alarm_time);
	for(int j=1; j<size; j++){
		temp = (table[j].tv_sec - table[j-1].tv_sec)+0.000001*(table[j].tv_usec - table[j-1].tv_usec);
		printf("%f\n",temp);
	}	


	temp = (table[size-1].tv_sec - table[0].tv_sec)+0.000001*(table[size-1].tv_usec - table[0].tv_usec);
	printf("\nElapsed time : %f\n", temp);
	return 0;
}