#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define OPT_ELF_HEADER 0x1
#define OPT_PRO_HEADER 0x2
#define OPT_SEC_HEADER 0x4

int main(int argc, char **argv){
	int c, opt = 0;
	char *exec = NULL;
	char cmd[256] = "readelf -";
	
	while((c = getopt(argc, argv, "eps")) != -1)
		switch(c){
			case 'e':
				opt |= OPT_ELF_HEADER;
				break;
			case 'p':
				opt |= OPT_PRO_HEADER;
				break;
			case 's':
				opt |= OPT_SEC_HEADER;
				break;
			case '?':
				if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	exec = argv[optind];

	if(exec == NULL){
		fprintf(stderr, "input file error\n");
		return -1;
	}
	
	if(opt == 0 || (opt & OPT_ELF_HEADER))
		strcat(cmd, "h");
	if(opt & OPT_PRO_HEADER)
		strcat(cmd, "l");
	if(opt & OPT_SEC_HEADER)
		strcat(cmd, "S");
	strcat(cmd, " ");
	strcat(cmd, exec);
	system(cmd);
	
	return 0;
}
