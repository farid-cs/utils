#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Param {
	char *argv0;
	size_t pathc;
	char **pathv;
} Param;

static bool
parse_arguments(Param *param, int argc, char **argv)
{
	int opt = 0;

	assert(argc > 0);
	assert(argv != nullptr);
	param->argv0 = argv[0];

	while ((opt = getopt(argc, argv, "u")) >= 0) {
		switch (opt) {
		case '?':
			fprintf(stderr, "usage: %s [-u] [file...]\n", argv[0]);
			return false;
		}
	}

	param->pathc = (size_t)(argc - optind);
	param->pathv = argv + optind;

	return true;
}

static void
handle_stream(FILE *stream)
{
	int byte = 0;

	while ((byte = fgetc(stream)) != EOF)
		if (fputc(byte, stdout) == EOF)
			break;
	assert(!ferror(stream));
	assert(!ferror(stdout));
}

static bool
handle_file(const char *path)
{
	FILE *stream = fopen(path, "r");

	if (stream == nullptr)
		return false;

	handle_stream(stream);
	assert(!fclose(stream));
	return true;
}

int
main(int argc, char **argv)
{
	Param param = {0};
	int status = EXIT_SUCCESS;

	if (!parse_arguments(&param, argc, argv))
		return EXIT_FAILURE;

	assert(!setvbuf(stdout, nullptr, _IONBF, 0));
	for (size_t i = 0; i != param.pathc; i++) {
		if (!strcmp(param.pathv[i], "-")) {
			handle_stream(stdin);
		} else if (!handle_file(param.pathv[i])) {
			fprintf(stderr, "%s: '%s': %s\n", param.argv0, param.pathv[i], strerror(errno));
			status = EXIT_FAILURE;
		}
	}
	if (param.pathc == 0)
		handle_stream(stdin);

	return status;
}
