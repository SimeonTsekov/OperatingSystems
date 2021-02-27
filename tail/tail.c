//------------------------------------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: tail.c
// FILE PURPOSE:
// prints the last 10 lines of a file
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 10

int tail(char *filename);
int print(char *content);

//------------------------------------------------------------------------
// FUNCTION: main
// calls the tail function, handles few errors and formats the output
// PARAMETERS:
// int argc - argument count
// char **argv - array with arguments
//------------------------------------------------------------------------u

int main(int argc, char **argv){
	if(argc==2 && argv[1][0]!='-'){	
		char *filename = argv[1]; 	
		if(argv[1][0]=='/'){	
			char *error = "tail: error reading '";	
			write(STDERR_FILENO, error, strlen(error));
			write(STDERR_FILENO, filename, strlen(filename));
			error = "': is a directory";
			write(STDERR_FILENO, error, strlen(error));
			write(STDERR_FILENO, "\n", 1);
		}else{
			int fd = open(filename, O_RDONLY);	
			if(fd<0){
				char *error = "tail: cannot open '";	
				write(STDERR_FILENO, error, strlen(error));
				write(STDERR_FILENO, filename, strlen(filename));
				perror("' for reading");
			}else{
				tail(filename);	
			}
			close(fd);
		}
	} else if(argc > 2) {
		for(int i=1; i<argc; i++){
			char *filename = argv[i];			
			
			if(strcmp(filename, "-")==0){
				int fd = open("help.txt", O_WRONLY | O_CREAT, 0777);
				char buff;

				while(1){	
					if(!read(STDIN_FILENO, &buff, 1)){break;}
					write(fd, &buff, 1);
				}

				close(fd);	
				tail("help.txt");
				remove("help.txt");

				if(i!=(argc-1)){
					write(STDOUT_FILENO, "\n", 1);		
				}

			} else {
				if(filename[0]=='/'){	
					char *error = "tail: error reading '";	
					write(STDERR_FILENO, error, strlen(error));
					write(STDERR_FILENO, filename, strlen(filename));
					error = "': is a directory";
					write(STDERR_FILENO, error, strlen(error));
					write(STDERR_FILENO, "\n", 1);

				}else{
					int fd = open(filename, O_RDONLY);	
					if(fd<0){
						char *error = "tail: cannot open '";	
						write(STDERR_FILENO, error, strlen(error));
						write(STDERR_FILENO, filename, strlen(filename));
						perror("' for reading");
					} else {
						if(strcmp(argv[i], "-")==0){
							int fd = open("help.txt", O_WRONLY | O_CREAT, 0777);
							char buff;

							while(1){	
								if(!read(STDIN_FILENO, &buff, 1)){break;}
								write(fd, &buff, 1);
							}

							close(fd);	
							tail("help.txt");
							remove("help.txt");
						} else {
							char filename_h[100] = "==> ";
							strcat(filename_h, filename);
							strcat(filename_h, " <==");
							strcat(filename_h, "\n");
							strcat(filename_h, "\0");

							write(STDOUT_FILENO, filename_h, strlen(filename_h));
							tail(filename);
						}
	
						if(i!=(argc-1)){
							write(STDOUT_FILENO, "\n", 1);		
						}
					}
					close(fd);
				}
			}
		}
	} else if(argc==1 || argv[1][0]=='-') {	
		int fd = open("help.txt", O_WRONLY | O_CREAT, 0777);
		char buff;

		while(1){	
			if(!read(STDIN_FILENO, &buff, 1)){break;}
			write(fd, &buff, 1);
		}

		close(fd);	

		tail("help.txt");

		remove("help.txt");
	}

	return 0;
}

//------------------------------------------------------------------------
// FUNCTION: tail
// outputs the last 10 lines of a file or all of them if the file has less than 10 lines, handles few errors
// PARAMETERS:
// char *filename - the name of the file, from which the function will read
//------------------------------------------------------------------------

int tail(char *filename){
	int fd = open(filename, O_RDONLY);
	
	char buff[1];
	char content[1000];
	int j = 1;
	int stop = 0;	
	
	lseek(fd, 2, SEEK_SET);
	if(!read(fd, buff, 1)){
		lseek(fd, 0 ,SEEK_SET);
		int read_value = read(fd, buff, 1);	
		if(read_value<0){
			char *error = "tail: error reading '";	
			write(STDERR_FILENO, error, strlen(error));
			write(STDERR_FILENO, filename, strlen(filename));
			perror("'");
			return 0;
		}
		write(STDOUT_FILENO, buff, 1);
		write(STDOUT_FILENO, "\n", 1);
		return 1;
	}

	lseek(fd, -2, SEEK_END);

	content[0] = '\n';
	int n = 0;

	for(int i=0; i<10; i++){	
		while(1){
			if(n==1000){break;}
			int read_value = read(fd, buff, 1);
			if(read_value<0){
				char *error = "tail: error reading '";	
				write(STDERR_FILENO, error, strlen(error));
				write(STDERR_FILENO, filename, strlen(filename));
				perror("'");
				return 0;
			}
			content[j] = buff[0];
			content[j+1] = '\0';
			j++;

			if(!lseek(fd, -2, SEEK_CUR)){
				stop = 1;
				lseek(fd, 0, SEEK_SET);
				int read_value = read(fd, buff, 1);
				if(read_value<0){
					char *error = "tail: error reading '";	
					write(STDERR_FILENO, error, strlen(error));
					write(STDERR_FILENO, filename, strlen(filename));
					perror("read");
					return 0;
				}
				content[j] = buff[0];
				content[j+1] = '\0';	
				break;
			}

			if(buff[0] == '\n') {break;}
		
			n++;
		}

		if(stop==1){break;}
	}
	
	if(stop == 0){
		content[j-1] = '\0';
	}

	print(content);

	ssize_t close_value = close(fd);
	if(close_value<0){
		char *error = "tail: error reading '";	
		write(STDERR_FILENO, error, strlen(error));
		write(STDERR_FILENO, filename, strlen(filename));
		perror("'");
	}
	return 1;
}

//------------------------------------------------------------------------
// FUNCTION: print
// prints the formatted from tail string
// PARAMETERS:
// char *content - the content to be printed, in reverse
//------------------------------------------------------------------------

int print(char *content){
	for(int i=strlen(content)-1; i>=0; i--){
		char buff = content[i];
		int write_value;
		int n = 0;
		while(1){
			write_value = write(STDOUT_FILENO, &buff, 1);
			if(write_value==1){break;}
			if(n==10){break;}
			n++;
		}
		if(write_value<=0){
			const char *error = "tail: error writing 'standard output'";	
			perror(error);	
			return 0;
		}
	}

	return 1;
}
