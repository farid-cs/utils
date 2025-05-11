#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t BUFSIZE = 1024;

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
	int status = EXIT_SUCCESS;
	int opt;

	while ((opt = getopt(argc, argv, "u")) >= 0) {
		switch (opt) {
		case 'u':
			BUFSIZE = 1;
			break;
		case '?':
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			return EXIT_FAILURE;
		}
	}
	for (int i = optind; i != argc; i++) {
		if (!strcmp(argv[i], "-")) {
			dump(stdin);
			continue;
		}
		stream = fopen(argv[i], "r");
		if (stream == NULL) {
			fprintf(stderr, "%s: '%s': %s\n", argv[0], argv[i], strerror(errno));
			status = EXIT_FAILURE;
			continue;
		}
		dump(stream);
		fclose(stream);
	}
	if (argc == optind)
		dump(stdin);
	return status;
}
