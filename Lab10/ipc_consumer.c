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

	pipe_id[0] = open(pipe_name[0], O_RDONLY, 0777);
	pipe_id[1] = open(pipe_name[1], O_WRONLY, 0777);

	if(pipe_id[0] == -1 || pipe_id[1] == -1){
		fprintf(stderr, "Could not open pipes \n");
		return -1;
	}

	while(1){
		memset(buffer, 0, BUF_SIZE);
		res = read(pipe_id[0], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "consumer Read error on pipe\n");
			return -1;
		}
		printf("%s\n", buffer);
		sleep(1);
		
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d 김형근", getpid());
		res = write(pipe_id[1], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "consumer Write error on pipe\n");
			return -1;
		}
	}

	close(pipe_id[0]);
	close(pipe_id[1]);
	return 0;
}
