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
		if(msgrcv(msgid, (void*)&data, sizeof(data)-sizeof(long int), 1, 0) == -1){
			fprintf(stderr, "msgrcv failed\n");
			return -1;
		}

		if(!strncmp(data.text, "exit", sizeof("exit")))
			break;
		printf("producer pid: %d, student_id: %d\n", data.pid, data.text);

		sleep(1);

		memset(buffer, 0, sizeof(buffer));
		strncpy(data.text, "김형근", BUF_SIZE);
		data.msg_type = 2;
		data.pid = getpid();

		if(msgsnd(msgid, (void*)&data, sizeof(data)-sizeof(long int), 0) == -1){
			fprintf(stderr, "msgsnd failed\n");
			return -1;
		}

		sleep(1);

	}
	
	return 0;
}
