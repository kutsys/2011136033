#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define THREAD_NUM 10

void *thread_function(void *arg);

int main(){
	int res[THREAD_NUM];
	pthread_t threads[THREAD_NUM];
	void *thread_result[THREAD_NUM];
	int i,j;

	for(i=0; i<THREAD_NUM; i++){
		res[i] = pthread_create(threads+i, NULL, thread_function, (void*)0);
		if(res[i]){
			fprintf(stderr, "pthread_crate[%d] appear error.(errno: %d)\n",i ,res[i]);
			for(j=0; j<i; i++)
				pthread_cancel(threads[j]);
			return -1;
		}
		printf("created pthread %d\n", i);
	}

	printf("2011136033 김형근\n");
	for(i=0; i<THREAD_NUM; i++){
		res[i] = pthread_join(threads[i], thread_result+i);
		if(res[i]){
			fprintf(stderr, "pthread_join[%d] apper error.(errno: %d)\n", i, res[i]);
			for(j=0; j<THREAD_NUM; j++)
				pthread_cancel(threads[j]);
			return -1;
		}
		printf("joined pthread %d\n", i);
	}

	printf("Finished(thread)\n");

	return 0;
}

void *thread_function(void *arg){
	system("./subproc");
	pthread_exit(0);
}
