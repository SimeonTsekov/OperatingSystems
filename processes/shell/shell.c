//--------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: shell.c
// FILE PURPOSE:
// program, replicating the functionality of linux's shell
//---------------------------------------------

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

char** parse_cmdline(const char *cmdline);

//--------------------------------------------
// FUNCTION: main
// creates a process in which calls execvp with a previously parsed 2D array of arguments
// repeats util Ctrl+D or 100 times
// PARAMETERS:
// none
//----------------------------------------------

int  main(){
	pid_t pid;
	int status;
	char cmdline[300];

	int i=0;

	while(1){
		if(write(STDOUT_FILENO, "$ ", 2)==-1){
			perror("write");
		}

		if(fgets(cmdline, 300, stdin) == NULL){
			break;
		}

		cmdline[strlen(cmdline)-1] = '\0';

		//input
		
		pid = fork();

		if(pid < 0){
			status = -1;
			perror("fork");

		} else if(pid==0){
			char **command = parse_cmdline(cmdline);

			if(execvp(command[0], command) == -1){
				perror(command[0]);

				for(int n=0; command[n]!=NULL; n++){
					free(command[n]);
					command[n] = NULL;
				}	
		
				free(command);
				command = NULL;
				exit(0);
				return 0;
			}

			for(int n=0; command[n]!=NULL; n++){
				free(command[n]);
				command[n] = NULL;
			}	
		
			free(command);
			command = NULL;
			i++;
		
			if(i==100){break;}	
		} else if(pid>0){
			if(waitpid(pid, &status, 0)==-1){
				perror("waitpid");
				return 0;
			}
		}		
	}
	
	return 0;
}

//--------------------------------------------
// FUNCTION: parse_cmdline
// recives a pointer to a string and parses and returns it in a form, suitable for execvp
// PARAMETERS:
// *cmdline - pointer to a string to be parsed and returned
//----------------------------------------------

char** parse_cmdline(const char *cmdline){
	int j1=0,j2=0;
	char **parse = malloc(sizeof(char*));
	parse[j1] = malloc(sizeof(char));
	
	for(int i=0; cmdline[i]!='\0'; i++, j2++){	
		parse[j1][j2] = cmdline[i];		
		parse[j1] = realloc(parse[j1], sizeof(char) * (i+2));

		if(cmdline[i]==' ' && i!=(strlen(cmdline)-1)){	
			parse[j1][j2] = 0;
			j1++;
			j2 = -1;
			parse = realloc(parse, sizeof(char*) * (j1+1));
			parse[j1] = malloc(sizeof(char));	
			
			while(1){
				if(cmdline[i+1]!=' '){break;}
				i++;
			}
		} 
		
		if (cmdline[i+1] == '\0' && cmdline[i]!=' '){
			parse[j1][j2+1] = 0;
			j1++;	
			parse = realloc(parse, sizeof(char*) * (j1+1));
		}
	}

	parse[j1] = 0;

	return parse;
}
