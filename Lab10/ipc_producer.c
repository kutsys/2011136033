#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RD_PATH "./cWrite"
#define WR_PATH "./pWrite"
#define BUF_SIZE 100

int main(){
	int pipe_id[2];
	int res;
	pid_t pid;
	char *buffer = (char*)malloc(sizeof(char)*BUF_SIZE);
	char *student_id = "2011136033\0";
	if(access(WR_PATH, F_OK) == -1){
		res = mkfifo(WR_PATH, 0777);
		if(res){
			fprintf(stderr, "Could not create fifo %s\n", WR_PATH);
			return -1;
		}
	}

	pipe_id[1] = open(WR_PATH, O_WRONLY);
	pipe_id[0] = open(RD_PATH, O_RDONLY);

	while(1){
		printf("Enter start for starting(or exit for end): ");
		memset(buffer, 0, BUF_SIZE);
		fgets(buffer, BUF_SIZE, stdin);
		if(!strcmp(buffer, "exit\n")){
			memset(buffer, 0, BUF_SIZE);
			strncpy(buffer, "exit", BUF_SIZE);
			break;
		}

		if(strcmp(buffer, "start\n")){
			continue;
		}
		sprintf(buffer, "producer pid: %d, student_id:  %s", getpid(), student_id);
		res = write(pipe_id[1], buffer, BUF_SIZE);
		if(res == -1){
			fprintf(stderr, "Write error on pipe\n");
			return -1;
		}
		memset(buffer, 0, BUF_SIZE);
		res = read(pipe_id[0], buffer, BUF_SIZE);
		printf("producer pid: %d, student_id: %s\n", getpid(), student_id);
		printf("%s\n", buffer);

	}
	
	free(buffer);
	close(pipe_id[0]);
	close(pipe_id[1]);
	return 0;
}
