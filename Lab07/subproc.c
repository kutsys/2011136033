#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void test_funct(){
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

int main(int argc, char **argv){
	int r;
	int file_descriptor;

	if(argc>1)
		sscanf(argv[1], "%d", &file_descriptor);
	else
		file_descriptor = -1;

	srand(getpid());
	while(1){
		test_funct();
		if(file_descriptor > -1)
			write(file_descriptor, "1", 1);
		r=rand()%10;
		sleep(r+1);
	}
	return 0;
}
