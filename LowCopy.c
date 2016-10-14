#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define SLEEP_SEC 1


int main(int argc, char *argv[]){
	char block[1024];
	int in, out;
	int nread;
	time_t st, et;

	st = time(NULL);
	if(argc > 3){
		printf("this program need parameters less than 3");
		exit(EXIT_FAILURE);
	}

	if(argc > 1){
		if((in = open(argv[1], O_RDONLY)) < 0){
			printf("dont exist file\n");
			exit(EXIT_FAILURE);
		}
	}	
	else{
		printf("we need to know what is input file\n");
		exit(EXIT_FAILURE);
	}

	if(argc > 2)
		out = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	else
		out = open("file.out", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

	while((nread = read(in,block,sizeof(block))) > 0){
		write(out, block, nread);
		et=time(NULL);
		if(et-st >= SLEEP_SEC){
			fflush(stdout);
			printf(".");
			st=et;
		}
	}
	printf("\ndone.\n");

	exit(EXIT_SUCCESS);
}
