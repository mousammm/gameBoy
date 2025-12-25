#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	if(argc < 2) {
		printf("usage gb <path/to/rom>\n");
		return 0;
	}
	printf("Hello world! %s %s\n", argv[0], argv[1]);
	return 0;
}
