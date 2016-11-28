#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void test_funct(int n){
	if(n>0){
		printf(".\n");
		return;
	}
	static int count = 0;
	pid_t pid = getpid();
	time_t theTime;
	struct tm *tm_ptr;

	theTime = time(NULL);
	tm_ptr = localtime(&theTime);

	count++;

	printf("pid: %d, time: %02d:%02d:%02d, count: %d\n", 
			pid, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, count);
}

int main(){
	srand(time(NULL));
	int r;

	while(1){
		r=rand()%10;
		sleep(r+1);
		test_funct(0);
	}
	return 0;
}
