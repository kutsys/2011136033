#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#define NUM_FORK 10
#define COUNT_DEST 20
#define BUF_SIZE 20

pid_t pid[NUM_FORK];
int count[NUM_FORK];

void unindentedExit(int sig){
	int i;
	for(i=0; i<NUM_FORK; i++){
		if(!pid[i])
			break;
		kill(pid[i], SIGKILL);
	}

	printf("Finished (process)\n");
	signal(sig, SIG_DFL);
	raise(sig);
}

int main(){
	int i, file_pipes[NUM_FORK*2];
	int overnum, readsize;
	char buffer[BUF_SIZE];
	memset(count, 0, sizeof(count));

	// regist signals
	signal(SIGKILL, unindentedExit);
	signal(SIGINT, unindentedExit);
	signal(SIGTERM, unindentedExit);
	signal(SIGQUIT, unindentedExit);
	signal(SIGHUP, unindentedExit);

	for(i=0;i<NUM_FORK;i++){
		if(!pipe(file_pipes+i*2)){		
			pid[i] = fork();
			switch(pid[i]){
			case -1:
				printf("%dth Fork Error!\n", i+1);
				return -1;
			case 0:	// child process
				sprintf(buffer, "%d", file_pipes[i*2+1]);
				execl("./subproc", "subproc", buffer,(char*) 0);
				return 0;
			}
		}
		else{
			printf("pipe Error!\n");
			return -1;
		}
	}

	// parent process
	while(1){
		overnum = 0;
		for(i=0; i<NUM_FORK; i++){
			memset(buffer, 0, sizeof(buffer));
			readsize = read(file_pipes[i*2], buffer, BUF_SIZE);
			count[i] += readsize>0?readsize:0;
			printf("%d:count[%d] = %d\n", pid[i], i, count[i]);
			if(count[i] >= COUNT_DEST){
				overnum++;
				if(pid[i] > 0){
					kill(pid[i], SIGKILL);
					pid[i] = -1;
				}
			}
		}
		if(overnum >= NUM_FORK)
			break;
	}
	printf("Finished (process)\n");
	return 0;
}
