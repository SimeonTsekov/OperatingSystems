#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

int main(){
	int pd[2];

    	if(pipe(pd)==-1){
    		perror("pipe");
		return 0;
	}

	pid_t pid = fork();
    	
	if(pid<0){
		perror("fork");
		return 0;
	} else if(pid==0) {
		char* arguments[2];

		arguments[0] = "ls";
		arguments[1] = "-l";
		arguments[2] = 0;
		
		dup2(pd[1], 1);	
		execvp("ls", arguments);
		exit(0);
	}

	dup2(pd[0], 0);
	close(pd[1]);  
	
	char* arguments2[2];
	
	arguments2[0] = "wc";
	arguments2[1] = "-l";
	arguments2[2] = 0;
	
	if(!execvp("wc", arguments2)){
		perror("exec");
		return 0;
	}
	exit(0);

	return 0;
}

