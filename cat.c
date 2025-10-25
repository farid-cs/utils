#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
dump(FILE *stream)
{
	int byte = 0;

	assert(!setvbuf(stream, NULL, _IONBF, 0));
	while ((byte = fgetc(stream)) != EOF)
		if (fputc(byte, stdout) == EOF)
			break;
	assert(!ferror(stream));
	assert(!ferror(stdout));
}

int
main(int argc, char **argv)
{
	FILE *stream = NULL;
	int status = EXIT_SUCCESS;
	int opt = 0;

	while ((opt = getopt(argc, argv, "u")) >= 0) {
		switch (opt) {
		case 'u':
			break;
		case '?':
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			return EXIT_FAILURE;
		}
	}
	assert(!setvbuf(stdout, NULL, _IONBF, 0));
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
		assert(!fclose(stream));
	}
	if (argc == optind)
		dump(stdin);
	return status;
}
