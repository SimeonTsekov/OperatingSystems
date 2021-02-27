#include <stdint.h>
#include <stdio.h>
#include <string.h>

void *read_image( const char *filepath );

int main( int argc, const char* argv[] ) {
	const char *filename = argv[1];
	char *buff = read_image(filename);

	uint32_t width;
	uint32_t height;

	memmove(&width, buff+18, sizeof(uint32_t));
	memmove(&height, buff+22, sizeof(uint32_t));

	printf("BMP image width: %d\n", width);
	printf("BMP image height: %d\n", height);
}
