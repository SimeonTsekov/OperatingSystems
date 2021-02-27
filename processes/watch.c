#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int  main(int argc, char **argv){
	pid_t pid;
	int status;

	char *command = argv[1];
	char *arguments[argc];

	int i;

	for(i=1; i<=argc; i++){
		arguments[i-1] = argv[i];
	}

	arguments[i] = NULL;

	while(1){
		pid = fork();

		if(pid < 0){
			status = -1;
			perror("fork");
		} else if(pid==0){
			sleep(2);
			if(execvp(command, arguments) == -1){
				perror(command);
				return 0;
			}
		} else if(pid>0){
			waitpid(pid, &status, 0);
		}		
	}
	
	return 0;
}
