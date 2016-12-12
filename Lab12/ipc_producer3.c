#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/msg.h>
#include <sys/types.h>

#define BUF_SIZE 20

struct msg_st{
	long int msg_type;
	pid_t pid;
	char text[BUF_SIZE];
};

int main(){
	struct msg_st data;
	int msgid;
	char buffer[BUF_SIZE];

	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		return -1;
	}

	while(1){
		printf("Enter start for starting(or exit for end): ");
		fgets(buffer, BUF_SIZE, stdin);
		
		if(!strcmp(buffer, "exit\n")){
			strncpy(data.text, "exit", BUF_SIZE);
			return -1;
		}

		if(strcmp(buffer, "start\n"))
			continue;
		memset(buffer, 0, sizeof(buffer));
		strncpy(data.text, "2011136033", BUF_SIZE);
		data.msg_type = 1;
		data.pid = getpid();

		if(msgsnd(msgid, (void*)&data, sizeof(data)-sizeof(long int), 0) == -1){
			fprintf(stderr, "msgsnd failed\n");
			return -1;
		}

		sleep(1);

		if(msgrcv(msgid, (void*)&data, sizeof(data)-sizeof(long int), 2, 0)== -1){
			fprintf(stderr, "msgrcv failed\n");
			return -1;
		}

		printf("producer pid: %d\n", getpid());
		printf("consumer pid: %d\n", data.pid);
		printf("%s %s\n", "2011136033", data.text);
	}
	
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		return -1;
	}

	return 0;
}
