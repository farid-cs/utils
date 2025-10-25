#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int pathc = 0;
char **pathv = NULL;

static bool
parse_arguments(int argc, char **argv)
{
	int opt = 0;

	while ((opt = getopt(argc, argv, "u")) >= 0) {
		switch (opt) {
		case 'u':
			break;
		case '?':
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			return false;
		}
	}

	pathc = argc - optind;
	pathv = argv + optind;

	return true;
}

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

	if (!parse_arguments(argc, argv))
		return EXIT_FAILURE;

	assert(!setvbuf(stdout, NULL, _IONBF, 0));
	for (int i = 0; i != pathc; i++) {
		if (!strcmp(pathv[i], "-")) {
			dump(stdin);
			continue;
		}
		stream = fopen(pathv[i], "r");
		if (stream == NULL) {
			fprintf(stderr, "%s: '%s': %s\n", argv[0], pathv[i], strerror(errno));
			status = EXIT_FAILURE;
			continue;
		}
		dump(stream);
		assert(!fclose(stream));
	}
	if (pathc == 0)
		dump(stdin);

	return status;
}
