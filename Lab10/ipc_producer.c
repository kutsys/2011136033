#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define BUF_SIZE 40

pid_t splitPid(char *buffer[]){
	int i, c=0;
	char temp[BUF_SIZE];

	for(i=0; i<BUF_SIZE; i++){
		c++;
		if((*buffer)[i] == ' ')
			break;
	}
	
	strncpy(temp, *buffer, sizeof(char)*c);
	strncpy(*buffer, *buffer+c, sizeof(char)*(BUF_SIZE-c));
	return (pid_t)atoi(temp);
}

int main(){
	pid_t pid;
	int res, pipe_id[2];
	char *pipe_name[2] = {"./prod_write", "./cons_write"}, buffer[BUF_SIZE+1];
	char *student_id = "201136033";

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
		
		
		pid = splitPid(&buffer);
		printf("producer pid: %d\n", getpid());
		printf("consumer pid: %d\n", pid);
		printf("student id: %s\n", student_id);
		printf("name: %s\n", buffer);
	}

	close(pipe_id[0]);
	close(pipe_id[1]);
	return 0;
}
