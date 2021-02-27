//--------------------------------------------
// NAME: Simeon Tsekov
// CLASS: XIa
// NUMBER: 24
// PROBLEM: #1
// FILE NAME: ls.c
// FILE PURPOSE:
// realisation of the UNIX system function ls
//---------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>


#define A_FLAG_MASK 0b0001
#define l_FLAG_MASK 0b0010
#define R_FLAG_MASK 0b0100


void set_flags(int argc, char **argv, short *options);
void read_current_dir(char *file_name, short flags);
long read_file(char *file_path, char *file_name, short flags, long blocks);
void print_type(mode_t mode);
int is_directory(mode_t mode);
void print_permissions(mode_t mode);
void print_user(struct stat st);
void print_group(struct stat st);
void print_time(struct stat st);
void rec(char *file_path, short flags);

//--------------------------------------------
// FUNCTION: main
// parses the input and executes the program
// PARAMETERS:
// int argc - argument count
// char **argv - arguments
//----------------------------------------------
int main(int argc, char **argv){
	DIR *dir;
	struct dirent *entry;
	char *file_name;
	short flags = 0b0000;

	//sets the flags and the opt index
	set_flags(argc, argv, &flags);

	//reccursive reading of directories
	if((flags & R_FLAG_MASK) && optind < argc){
		for(int i=optind; i<argc; i++){
			rec(argv[i], flags);
		}
		return 0;
	} else if((flags & R_FLAG_MASK) && optind == argc){
		rec(".", flags);
		return 0;
	}

	//reads current directory when no arguments are assigned
	if(optind == argc){
		read_current_dir(".",flags);
		return 0;
	}

	for(int i=optind; i<argc; i++){
		file_name = argv[i];
		struct stat st;

		if(stat(file_name, &st) == -1){
			char message[PATH_MAX];
			if(errno == ENOENT){
				strcpy(message,"ls: cannot access ");
				strcat(message, file_name);
				perror(message);
			} else if(errno == EACCES){
				strcpy(message,"ls: cannot open directory ");
				strcat(message, file_name);
				perror(message);
			}
			return 0;
		}

		if(is_directory(st.st_mode)){
			if(!(dir = opendir(file_name))){
				perror("ls");
				return 0;
			}

			if(i>optind){
				printf("\n");
			}
			if(argc-1 > optind){
				printf("%s:\n", file_name);
			}
			read_current_dir(file_name, flags);
			closedir(dir);
		} else {
			if((dir=opendir(argv[i-1]))){
				closedir(dir);
				printf("\n");
			}

			dir = opendir(".");
	
			while((entry = readdir(dir))){
				if(strcmp(entry->d_name, file_name) == 0){
					read_file(file_name, file_name, flags, 0);
				}
			}

			closedir(dir);
		}

	}
	return 0;
}

//--------------------------------------------
// FUNCTION: set_flags
// sets the flags for the command
// PARAMETERS:
// int argc - argument count
// char **argv - arguments
// short *flags - a binary short for the flags
//----------------------------------------------
void set_flags(int argc, char **argv, short *flags){
	int opt;

	while((opt = getopt(argc, argv, "AlR")) != -1){
		switch(opt){
			case 'A':
				*flags |= A_FLAG_MASK; 
				break;
			case 'l':
				*flags |= l_FLAG_MASK;
				break;
			case 'R':
				*flags |= R_FLAG_MASK;
				break;
		}
	}
}

//--------------------------------------------
// FUNCTION: read_current_dir
// print information about the directory
// PARAMETERS:
// char *file_name - the name/path of the directory
// short flags - a binary value for the flags
//----------------------------------------------
void read_current_dir(char *file_name, short flags){
	DIR *dir;
	if(!(dir = opendir(file_name))){
		char message[PATH_MAX];
		if(errno == ENOENT){
			strcpy(message,"ls: cannot access ");
			strcat(message, file_name);
			perror(message);
		} else if(errno == EACCES){
			strcpy(message,"ls: cannot open directory ");
			strcat(message, file_name);
			perror(message);
		}
		return;
	}
	struct dirent *entry;
	long blocks = 0;
	char buff[PATH_MAX];

	while((entry = readdir(dir))){
		strcpy(buff, file_name);
		strcat(buff, "/");
		strcat(buff, entry->d_name);
		if(strcmp(entry->d_name,".")!=0 && 
				strcmp(entry->d_name,"..")!=0){
			if(entry->d_name[0] != '.' || (flags & A_FLAG_MASK)){
				blocks = read_file(buff, entry->d_name ,flags, blocks);	
			}
		}

	}
	
	if(flags & l_FLAG_MASK){
		printf("total %ld\n", blocks/2);
	}

	closedir(dir);
}

//--------------------------------------------
// FUNCTION: read_file
// prints information about the file
// PARAMETERS:
// char *file_path - path to the file
// char *file_name - name of the file
// short flags - a binary short for the flags
// long blocks - a long value for the block count
//----------------------------------------------
long read_file(char *file_path, char *file_name, short flags, long blocks){
	struct stat st;
	stat(file_path, &st);

	print_type(st.st_mode);

	if(flags & l_FLAG_MASK){
		print_permissions(st.st_mode);
		printf(" ");
		printf("%lu", st.st_nlink);
		printf(" ");
		print_user(st);
		printf(" ");
		print_group(st);
		printf(" ");
		printf("%ld", st.st_size);
		printf(" ");
		print_time(st);
	}

	printf(" ");
	printf("%s\n", file_name);	
	
	return blocks += st.st_blocks;
}

//--------------------------------------------
// FUNCTION: print_type
// prints the type of the file
// PARAMETERS:
// mode_t mode - a struct, containing info about the mode of the file
//----------------------------------------------
void print_type(mode_t mode){
	if(S_ISREG(mode)){
		printf("-");
	} else if(S_ISDIR(mode)){
		printf("d");
	} else if(S_ISBLK(mode)){
		printf("b");
	} else if(S_ISCHR(mode)){
		printf("c");
	} else if(S_ISFIFO(mode)){
		printf("p");
	} else if(S_ISLNK(mode)){
		printf("l");
	} else if(S_ISSOCK(mode)){
		printf("s");
	}
}

//--------------------------------------------
// FUNCTION: is_directory
// checks if the file is a directory
// PARAMETERS:
// mode_t mode - a struct. containing info about the mode of the file
//----------------------------------------------
int is_directory(mode_t mode){
	return mode & S_IFDIR;
}

//--------------------------------------------
// FUNCTION: print_permissions
// prints the permissions for the file
// PARAMETERS:
// mode_t mode - a struct, containing info about the mode of the file
//----------------------------------------------
void print_permissions(mode_t mode){
    putchar((mode & S_IRUSR) ? 
		    'r' : '-');
    putchar((mode & S_IWUSR) ? 
		    'w' : '-');
    putchar((mode & S_IXUSR) 
		    ? 'x' : '-');
    putchar((mode & S_IRGRP) 
		    ? 'r' : '-');
    putchar((mode & S_IWGRP) 
		    ? 'w' : '-');
    putchar((mode & S_IXGRP) 
		    ? 'x' : '-');
    putchar((mode & S_IROTH) 
		    ? 'r' : '-');
    putchar((mode & S_IWOTH) 
		    ? 'w' : '-');
    putchar((mode & S_IXOTH) 
		    ? 'x' : '-');
}

//--------------------------------------------
// FUNCTION: print_user
// prints the user
// PARAMETERS:   
// stat st - a struct, containing info about the file
//----------------------------------------------
void print_user(struct stat st){
	struct passwd *pwd;
	pwd = getpwuid(st.st_uid);
	printf("%s", pwd->pw_name);
}

//--------------------------------------------
// FUNCTION: print_group
// prints the group
// PARAMETERS:   
// stat st - a struct, containing info about the file
//----------------------------------------------
void print_group(struct stat st){
	struct group *gr;
	gr = getgrgid(st.st_gid);
	printf("%s", gr->gr_name);
}

//--------------------------------------------
// FUNCTION: print_time
// prints the time of the last modification
// PARAMETERS:   
// stat st - a struct, containing info about the file
//----------------------------------------------
void print_time(struct stat st){
	time_t time = st.st_mtime;
	struct tm *tm;
	char buff[256];

	tm = localtime(&time);

	strftime(buff, sizeof(buff), "%b %e %H:%M", tm);
	printf("%s", buff);

}

//--------------------------------------------
// FUNCTION: rec
// traverses the directories recursively
// PARAMETERS:   
// char *file_path - the path of the file
// short flags - a binary formatted short, containing info about the flags
//----------------------------------------------
void rec(char *file_path, short flags){
	struct stat st;
	char **directories = malloc(sizeof(char*));
	int directory_count = 0;
	long blocks = 0;
	DIR *dir;
	struct dirent *entry;

       	if(!(dir = opendir(file_path))){
		char message[PATH_MAX];
		if(errno == ENOENT){
			strcpy(message,"ls: cannot access ");
			strcat(message, file_path);
			perror(message);
		} else if(errno == EACCES){
			strcpy(message,"ls: cannot open directory ");
			strcat(message, file_path);
			perror(message);
		}
		return;
	}

	printf("%s:\n", file_path);
	while((entry = readdir(dir))){
		char file_name[PATH_MAX];
		strcpy(file_name, file_path);
		if(strcmp(file_name, "/")!=0){
			strcat(file_name, "/");
		}
		strcat(file_name, entry->d_name);
		stat(file_name, &st);
		
		if(strcmp(entry->d_name,".")!=0 && 
				strcmp(entry->d_name,"..")!=0){
			if(entry->d_name[0] != '.' || (flags & A_FLAG_MASK)){

				blocks += read_file(file_name, entry->d_name, flags, 0);
	
				if(is_directory(st.st_mode)){
					directories = realloc(directories, sizeof(char*) * (directory_count + 1));
					directories[directory_count] = malloc(sizeof(char) * (strlen(file_name) + 1));
					strcpy(directories[directory_count++], file_name);
				}
			}
		}
	}

	if(flags & l_FLAG_MASK){
		printf("total %ld\n", blocks/2);
	}

	for(int i=0; i<directory_count; i++){
		printf("\n");
		rec(directories[i], flags);
		free(directories[i]);
	}

	free(directories);
	closedir(dir);
}
