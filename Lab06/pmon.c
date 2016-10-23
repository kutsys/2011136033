#include<sys/types.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

// get PID(process id) belong processName
pid_t getPid(char *processName){
	FILE *pRead;
	int len;
	char buffer[4][20];

	if((pRead = popen("ps -a", "r")) == NULL){
		fputs("popen Error", stderr);
		return -1;
	}

	while((len = fscanf(pRead, "%s%*s%*s%s", buffer[0], buffer[1]))>0){
		if(!strcmp(buffer[1], processName)){
			pclose(pRead);
			return (pid_t)atoi(buffer[0]);
		}
		
	}

	pclose(pRead);
	return -1;
}

void intervalAlarm(int sig){
	pid_t pid;
	pid = getPid("ptest");

	fflush(stdout);
	if(pid < 0)
		printf("not existed\n");
	else
		printf("running\n");
	
	alarm(5);
}

int main(int argc, char* argv[]){
	pid_t  pid;
	char command;
	struct sigaction act;
	
	act.sa_handler = intervalAlarm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, NULL);
	
	alarm(5);

	while(1){
		pid = getPid("ptest");
		printf(">>");
		scanf("%c", &command);

		switch(command){
		case 'Q':
		case 'q':
			printf("exit pmon\n");
			if(pid >= 0)
				printf("warning: ptest is still running\n");
			exit(EXIT_SUCCESS);
		case 'K':
		case 'k':
			if(pid >= 0)
				kill(pid, SIGKILL);
			break;
		case 'S':
		case 's':
			if(pid >= 0){
				printf("already executed\n");
			}
			else{
				system("./ptest &");
			}
			break;
		case 'R':
		case 'r':
			if(pid >= 0)
				kill(pid, SIGKILL);
			system("./ptest &");
		}
	}

	exit(EXIT_SUCCESS);
}
