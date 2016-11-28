#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
	pid_t pid;
	int count = 0;

	

	while(1){
		if(count >= 20)
			break;

		getc(stdin);
		fflush(stdin);

		pid = fork();
		switch(pid){
		case -1:
			perror("fork() Error!");
			return -1;
		case 0:
			printf("child: %d\n", getpid());
			return 0;
		default:
			count ++;
			printf("parent: %d, count: %d\n", getpid(), count);
		}
	}
	return 0;
}
