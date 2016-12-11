#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define BUF_SIZE 40

int main(){
	pid_t pid;
	int res, pipe_id[2];
	char *pipe_name[2] = {"./prod_write", "./cons_write"}, buffer[BUF_SIZE+1];

	if(access(pipe_name[0], F_OK) == -1){
		res = mkfifo(pipe_name[0], 0777);
		if(res){
			fprintf(stderr, "Could not create read fifo %s\n", pipe_name[0]);
			return -1;
		}
	}

	if(access(pipe_name[1], F_OK) == -1){
		res = mkfifo(pipe_name[1], 0777);
		if(res){
			fprintf(stderr, "Could not create write fifo %s\n", pipe_name[1]);
			return -1;
		}
	}


	pipe_id[0] = open(pipe_name[1], O_RDONLY, 0777);

	printf("1\n");
	pipe_id[1] = open(pipe_name[0], O_WRONLY, 0777);

	printf("2\n");

	if(pipe_id[0] == -1 || pipe_id[1] == -1){
		fprintf(stderr, "Could not open pipes \n");
		return -1;
	}

	printf("3\n");

	while(1){
		printf("while start");
		memset(buffer, 0, sizeof(0));
		scanf("%s", buffer);
		if(!strcmp(buffer, "exit"))
			break;
		if(strcmp(buffer, "start"))
			continue;

		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "2011136033, pid: %d", getpid());
		res = write(pipe_id[1], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "producer Write error on pipe\n");
			return -1;
		}
		sleep(1);

		memset(buffer, 0, sizeof(buffer));
 		res = read(pipe_id[0], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "producer Read error on pipe\n");
			return -1;
		}
		pid = atoi(buffer);
		printf("producer pid: %d\n", getpid());
		printf("consumer pid: %d\n", pid);
	}

	close(pipe_id[0]);
	close(pipe_id[1]);
	return 0;
}
