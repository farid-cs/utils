#include <stdio.h>

int
main(int argc, char **argv)
{
	if (argc < 2)
		for (;;)
			printf("y\n");
	for (;;) {
		for (int i = 1; i != argc - 1; i++)
			printf("%s ", argv[i]);
		printf("%s\n", argv[argc-1]);
	}
}
