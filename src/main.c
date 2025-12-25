#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <path/to/rom>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *gb_rom_file = fopen(argv[1], "rb");
	printf("%p\n", gb_rom_file);

	printf("Hello world! %s %s\n", argv[0], argv[1]);
	exit(EXIT_SUCCESS);
}
