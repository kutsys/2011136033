#include<stdio.h>
#include<stdlib.h>


int main(int argc, char* argv[]){
	printf("$HOME:%s\n", getenv("HOME"));
	printf("$PS1:%s\n", getenv("PS1"));		// in bash, PS1 and PS2 are shell variables, not environment variables(in least normally)
	printf("$PATH:%s\n", getenv("PATH"));
	printf("$LD_LIBRARY_PATH:%s\n", getenv("LD_LIBRARY_PATH"));

	setenv("TEST_ENV","1234", 0);
	
	printf("setenv() can't export environment values from this process to calling process(like shell)\n");
	printf("So print here\n");
	printf("$TEST_ENV: %s\n", getenv("TEST_ENV"));

	exit(EXIT_SUCCESS);
}
