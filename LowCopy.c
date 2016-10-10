#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define SLEEP_MIC 10

typedef struct _file{
	char *input;
	char *output;
}File;

void *copyFile(void *file){
	File *fp = (File*)file;
	char block[1024];
	int in, out;
	int nread;
	
	in = open(fp->input, O_RDONLY);
	out = open(fp->output, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	while((nread = read(in,block,sizeof(block))) > 0)
		write(out, block, nread);
}

void *printDot(void *arg){
	while(1){
		usleep(SLEEP_MIC);
		printf(".");
		fflush(stdout);
	}
}

int main(int argc, char *argv[]){
	File f;
	int i;
	pthread_t tid[2];
	int iret[2];

	if(argc > 3){
		printf("this program need parameters less than 3(input and output)\n");
		exit(1);
	}
	if(argc>1)
		f.input = argv[1];
	else
		f.input = "file.in";

	if(argc>2)
		f.output = argv[2];
	else
		f.output = "file.out";


	if(iret[0] = pthread_create(&(tid[0]), NULL, copyFile, (void*)&f)){
		fprintf(stderr, "Error: pthread_create() return code %d\n", iret[0]);
		exit(EXIT_FAILURE);
	}
	if(iret[1] = pthread_create(&(tid[1]), NULL, printDot, NULL)){
		fprintf(stderr, "Error: pthread_create() return code %d\n", iret[1]);
		exit(EXIT_FAILURE);
	}

	pthread_join(tid[0], NULL);
	pthread_cancel(tid[1]);
	printf("\ndone\n");

	exit(0);
}
