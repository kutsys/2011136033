#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#define NUM_FORK 10
#define COUNT_DEST 5
#define BUF_SIZE 20

pid_t pid[NUM_FORK];
int count[NUM_FORK];

void z_handler(){
	while(waitpid(-1,NULL, WNOHANG) > 0);
}

void unindentedExit(int sig){
	int i;
	for(i=0; i<NUM_FORK; i++){
		if(pid[i]>0)
			kill(pid[i], SIGKILL);
	}

	printf("Finished (process)%d\n", sig);
	signal(sig, SIG_DFL);
	raise(sig);
}

int main(){
	int i, j, file_pipes[NUM_FORK*2];
	int overnum, readsize;
	char buffer[BUF_SIZE];
	memset(count, 0, sizeof(count));

	// regist signals
	signal(SIGKILL, unindentedExit);
	signal(SIGINT, unindentedExit);
	signal(SIGTERM, unindentedExit);
	signal(SIGQUIT, unindentedExit);
	signal(SIGHUP, unindentedExit);

	signal(SIGCHLD, z_handler);

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
		printf("2011136033 김형근\n");
		overnum = 0;
		for(i=0; i<NUM_FORK; i++){
			if(pid[i] > 0){
				write(file_pipes[i*2+1], '2', 1);
				readsize = 0;
				readsize = read(file_pipes[i*2], buffer, sizeof(buffer));
			
				for(j=0; j<readsize; j++){
					if(buffer[j] == '1')
						count[i]++;
					else if(buffer[j] == '0')
						pid[i] = 0;
				}
				if(count[i] >= COUNT_DEST){
					kill(pid[i], SIGKILL);
					pid[i] = 0;
				}
			}
			if(pid[i] <=0)
				overnum++;
		}
		
		printf("overnum: %d\n",overnum);
		if(overnum >= NUM_FORK)
			break;
	}
	printf("Finished (process)\n");
	return 0;
}
