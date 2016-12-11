#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#define DEST_COUNT 20
#define SLEEP_SEC 3

int call_times;
pthread_mutex_t work_mutex;


#define THREAD_NUM 10
pthread_t threads[THREAD_NUM];

void *subproc(void *arg){
	int r, i;
	int count = 0;

	srand(time(NULL)+getpid());
	
	while(count<DEST_COUNT){
		if(count <= call_times/THREAD_NUM){
			pthread_mutex_lock(&work_mutex);
			call_times++;
			count = test_funct(count);
			pthread_mutex_unlock(&work_mutex);

			r = rand() % SLEEP_SEC + 1;
			sleep(r);
		}
	}
	pthread_exit(0);
}

int test_funct(int count){
	int result_count = count;
	pid_t pid = getpid();
	time_t the_time;
	struct tm *tm_ptr;

	the_time = time(NULL);
	tm_ptr = localtime(&the_time);

	result_count++;

	printf("pid: %d, time: %02d:%02d:%02d, count: %d\n", 
			pid, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, result_count);
	return result_count;
}

int main(){
	int res;
	void *thread_result;
	int i, j;

	res = pthread_mutex_init(&work_mutex, NULL);
	if(res){
		fprintf(stderr, "Mutex initialization failed(errno: %d)\n", res);
		return -1;
	}
	
	pthread_mutex_lock(&work_mutex);
	printf("2011136033 김형근\n");

	for(i=0; i<THREAD_NUM; i++){
		res = pthread_create(threads+i, NULL, subproc, NULL);
		if(res){
			fprintf(stderr, "%dth pthread create failed(errno: %d)\n", i+1, res);
			for(j=0; j<i; j++)
				pthread_cancel(threads[j]);
			pthread_mutex_destroy(&work_mutex);
			return -1;
		}
	}

	pthread_mutex_unlock(&work_mutex);
	for(i=0; i<THREAD_NUM; i++){
		res = pthread_join(threads[i], &thread_result);
		if(res){
			fprintf(stderr, "%dth pthread join failed(errno: %d)\n", i+1, res);
		}
	}

	printf("Finished Successfully\n");
	pthread_mutex_destroy(&work_mutex);

	return 0;
}


