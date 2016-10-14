#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>

#define SLEEP_SEC 1

int main(int argc, char *argv[]){
	char block[1024];
	size_t nSize;
	FILE *input, *output;
	time_t st,et;	

	st = time(NULL);
	
	if(argc > 3){
		fputs("this Program need parameters less than 3\n", stderr);
		exit(EXIT_FAILURE);
	}
	
	if(argc > 1){
		if((input = fopen(argv[1], "rb")) == NULL){
			fputs("don't exist file!!\n", stderr);
			exit(EXIT_FAILURE);
		}
	}
	else{
		fputs("we need to know what is input file!\n", stderr);
		exit(EXIT_FAILURE);
	}

	if(argc > 2)
		output = fopen(argv[2], "wb");
	else
		output = fopen("file.out", "wb");

	
	while(0 < (nSize = fread(block, sizeof(char), 1024, input))){
		fwrite(block, 1, nSize, output);
		et = time(NULL);
		if(et-st >= SLEEP_SEC){
			fflush(stdout);
			fputc('*', stdout);
			st = et;
		}
	}
	fputs("\ndone.\n",stdout);

	fclose(input);
	fclose(output);

	exit(EXIT_SUCCESS);
}
