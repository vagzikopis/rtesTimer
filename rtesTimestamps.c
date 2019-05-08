
										
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
int flag = 1;

static void ualarm_handler(int signum){
	flag = 0;	
}

static void alarm_handler(int sig){
	if (alarm_usec>0){
		signal(SIGALRM, ualarm_handler);
		ualarm(alarm_usec, 0);
	}else{
		flag = 0;
	}
}


int main(){
	struct timeval tv;
	float alarm_time, sleep_time, temp, dec;
	int alarm_sec, sleep_sec, sleep_usec, size;
// uncomment filename and fd variable and enter your path to save data //
	/*char filename[] = "data/final_timestamps";					
	int fd;*/
	printf("Enter total sampling time: ");
	scanf("%f", &alarm_time);
	printf("Enter interval time: ");
	scanf("%f", &sleep_time);


	temp = (alarm_time / sleep_time);
	size = (int)temp;
	dec = temp-size;
	if (dec != 0.0f){
		size++;
	}
	struct timeval table[size];

	alarm_sec = (int)(alarm_time);
	temp =(alarm_time-alarm_sec)*1000000;
	alarm_usec = (int)temp;
	sleep_sec = (int)(sleep_time);
	temp = (sleep_time - sleep_sec)*1000000;
	sleep_usec = (int)temp;


	printf("Alarm seconds: %d\n", alarm_sec);
	printf("Alarm useconds: %d\n", alarm_usec);
	printf("Sleep seconds: %d\n", sleep_sec);
	printf("Sleep useconds: %d\n\n", sleep_usec);
	
	signal(SIGALRM, alarm_handler);

	if (alarm_sec != 0){
		alarm(alarm_sec);
	} else {
		ualarm(alarm_usec, 0);
	}
	int fixed = sleep_usec;
	int shift = 0;
	int i = 0;
	float sum = 0;
	temp = 0;
	while(flag){
		gettimeofday(&tv, NULL);
		table[i].tv_sec = tv.tv_sec;
		table[i].tv_usec = tv.tv_usec;
		if (i>0){
			sum = sum + ((table[i].tv_sec - table[i-1].tv_sec)+0.000001*(table[i].tv_usec - table[i-1].tv_usec) - 0.1);
			temp = (sum / i); // calculate average error //
			if (sleep_usec > 0){
				shift = (int)(temp*1000000);
			}
		}
		sleep_usec = fixed - shift; // dynamically change usleep // 
		i++;
		sleep(sleep_sec);
		usleep(sleep_usec);
	}

// Optional step, used to save data to a txt file //
/*	fd = creat(filename, 0644);	 
	dup2(fd, STDOUT_FILENO);
	close(fd);*/ 
	printf("Total Sampling Time: %f\n", alarm_time);
	printf("Interval Time: %f\n\n", sleep_time);
	for(int j=1; j<i; j++){
		temp = (table[j].tv_sec - table[j-1].tv_sec)+0.000001*(table[j].tv_usec - table[j-1].tv_usec);
		printf("%f\n",temp);
	}	
	return 0;
}
