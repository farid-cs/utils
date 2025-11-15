#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static size_t pathc = 0;
static char **pathv = nullptr;

static bool
parse_arguments(int argc, char **argv)
{
	int opt = 0;

	assert(argc > 0);
	assert(argv != nullptr);
	while ((opt = getopt(argc, argv, "u")) >= 0) {
		switch (opt) {
		case '?':
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			return false;
		}
	}

	pathc = (size_t)(argc - optind);
	pathv = argv + optind;

	return true;
}

static void
dump(FILE *stream)
{
	int byte = 0;

	assert(!setvbuf(stream, nullptr, _IONBF, 0));
	while ((byte = fgetc(stream)) != EOF)
		if (fputc(byte, stdout) == EOF)
			break;
	assert(!ferror(stream));
	assert(!ferror(stdout));
}

int
main(int argc, char **argv)
{
	FILE *stream = nullptr;
	int status = EXIT_SUCCESS;

	if (!parse_arguments(argc, argv))
		return EXIT_FAILURE;

	assert(!setvbuf(stdout, nullptr, _IONBF, 0));
	for (size_t i = 0; i != pathc; i++) {
		if (!strcmp(pathv[i], "-")) {
			dump(stdin);
			continue;
		}
		stream = fopen(pathv[i], "r");
		if (stream == nullptr) {
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
