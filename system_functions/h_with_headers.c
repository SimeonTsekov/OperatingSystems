#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1000

int main(int argc, char **argv){
	if(argc > 2){
		for(int i=1; i<argc; i++){	
			char filename_arr[100];
			filename_arr[0] = '\0';
			char *filename;
			filename = '\0';
			
			filename = argv[i];

			strcat(filename_arr, "==> ");
			strcat(filename_arr, filename);
			strcat(filename_arr, " <==");
			strcat(filename_arr, "\n");
			strcat(filename_arr, "\0");

			write(STDOUT_FILENO, filename_arr, strlen(filename_arr));

			char buff[BUFFER_SIZE];
			buff[0] = '\0';
			
			int fd = open(argv[i], O_RDONLY);
			read(fd, buff, BUFFER_SIZE);

			int j = 0;
			int new_lines = 0;

			while(1){
				if(buff[j] == '\n'){new_lines++;}
				if(new_lines == 10){break;}

				j++;
			}

			write(STDOUT_FILENO, buff, j);	
			
			write(STDOUT_FILENO, "\n", 1);		
			
			if(i!=(argc-1)){
				write(STDOUT_FILENO, "\n", 1);		
			}

			close(fd);
		}
	} else {
		char buff[BUFFER_SIZE];

		int fd = open(argv[1], O_RDONLY);
		read(fd, buff, BUFFER_SIZE);

		int j = 0;
		int new_lines = 0;

		while(1){
			if(buff[j] == '\n'){new_lines++;}
			if(new_lines == 10){break;}

			j++;
		}

		write(STDOUT_FILENO, buff, j);
		
		write(STDOUT_FILENO, "\n", 1);		
		
		close(fd);
	}
	return 0;
}
