#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#define BUF_SIZE 20

struct shared_use_st{
	int pid[2];
	char student_id[BUF_SIZE];
	char name[BUF_SIZE];
	int written_by_you;
};

int main(){
	void *shared_memory = (void*)0;
	struct shared_use_st *shared_stuff;
	char buffer[BUF_SIZE];
	int shmid, res;	

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if(shmid == -1){
		fprintf(stderr, "shmget failed\n");
		return -1;
	}
	shared_memory = shmat(shmid, (void*)0, 0);
	if(shared_memory == (void*)-1){
		fprintf(stderr, "shmat failed\n");
		return -1;
	}

	shared_stuff = (struct shared_use_st*)shared_memory;
	shared_stuff->written_by_you = 0;

	while(1){
		while(shared_stuff->written_by_you == 1){
			sleep(1);
		}
		
		printf("Enter start for starting(or exit for end): ");
		fgets(buffer, BUF_SIZE, stdin);
		if(!strcmp(buffer, "exit\n")){
			strncpy(shared_stuff->student_id, "exit", BUF_SIZE);
			shared_stuff->written_by_you = 1;
			break;
		}

		if(strcmp(buffer, "start\n")){
			continue;
		}

		shared_stuff->pid[0] = getpid();
		strncpy(shared_stuff->student_id, "2011136033",BUF_SIZE);
		shared_stuff->written_by_you = 1;

		while(shared_stuff->written_by_you == 1){
			sleep(1);
		}
		printf("producer pid: %d\n", shared_stuff->pid[0]);
		printf("consumer pid: %d\n", shared_stuff->pid[1]);
		printf("%s %s\n", shared_stuff->student_id, shared_stuff->name);
	}

	if(shmdt(shared_memory) == -1){
		fprintf(stderr, "shmdt failed\n");
		return -1;
	}

	return 0;
}
