#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#define DEST_NUM 20

int file_descriptor;
int count;

void unindentedExit(int sig){
	int i;
	if(file_descriptor > 0)
		write(file_descriptor, "0", 1);
	
	signal(sig, SIG_DFL);
	raise(sig);
}

void test_funct(){
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

	// regist signals
	signal(SIGKILL, unindentedExit);
	signal(SIGINT, unindentedExit);
	signal(SIGTERM, unindentedExit);
	signal(SIGQUIT, unindentedExit);
	signal(SIGHUP, unindentedExit);
	

	// get file_descriptor
	if(argc>1)
		sscanf(argv[1], "%d", &file_descriptor);
	else
		file_descriptor = 0;

	// randomly call test_funct();
	srand(getpid()+time(NULL));
	while(1){
		test_funct();

		if(count >= DEST_NUM)
			break;

		if(file_descriptor > 0){
			write(file_descriptor, "1", 1);
		}
		r=rand()%10;
		sleep(r+1);
	}
	return 0;
}
