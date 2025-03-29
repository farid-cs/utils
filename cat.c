#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

static int
dump(FILE *stream)
{
	char buf[BUFSIZE];
	size_t bytes;

	while ((bytes = fread(buf, 1, BUFSIZE, stream)) == BUFSIZE)
		fwrite(buf, 1, BUFSIZE, stdout);
	if (feof(stream))
		fwrite(buf, 1, bytes, stdout);
	return 0;
}

int
main(int argc, char **argv)
{
	FILE *stream;

	if (argc < 2) {
		dump(stdin);
		return EXIT_SUCCESS;
	}

	for (int i = 1; i != argc; i++) {
		stream = fopen(argv[i], "r");
		dump(stream);
		fclose(stream);
	}

	return EXIT_SUCCESS;
}
