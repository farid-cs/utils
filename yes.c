#include <stdio.h>

int
main(int argc, char **argv)
{
	const char *s = "y";

	if (argc > 1)
		s = argv[1];
	for (;;)
		printf("%s\n", s);
}
