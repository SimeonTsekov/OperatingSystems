#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void *read_image(const char *filepath);
int has_a_format(const char *filename);
int is_a_png(const char *filename);

int main( int argc, const char* argv[] ) {
	if(argc==1){
		printf("Unable to open file\n");
		return 0;
	}

	const char *filename = argv[1];
	int is_png = is_a_png(filename);
	char *buff = read_image(filename);

	if(!has_a_format(filename)){
		printf("Unable to open file\n");
		return 0;
	}/* else if (is_png!=0){
		printf("Unable to parse file\n");
		return 0;
	} else if(buff == NULL){
		printf("Unable to open file\n");
		return 0;
	}*/

	uint32_t width;
	uint32_t height;
	long int size;

	memmove(&width, buff+16, sizeof(uint32_t));
	width = ntohl(width);
	memmove(&height, buff+20, sizeof(uint32_t));
	height = ntohl(height);

	FILE *fp = fopen(filename, "r");
	fseek(fp, 0L, SEEK_END); 
	size = ftell(fp); 

	printf("PNG image width: %d\n", width);
	printf("PNG image height: %d\n", height);
	printf("PNG file size: %ld\n", size);

	fclose(fp);
}

int has_a_format(const char *filename){
	for(int i=0; i<strlen(filename); i++){
		if(filename[i] == '.')
			return 1;
	}

	return 0;
}

int is_a_png(const char *filename){
	int i;
	char file_type[4];
	for(i=0; filename[i]!='.'; i++);
	int a = i+4;
	int j;

	for(j=0; i<a; i++, j++){
		file_type[j] = filename[i];
	}

	file_type[j] = '\0';
	
	return (strcmp(".png", file_type));
}
