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

static int
run(Param *param)
{
	FILE *stream = nullptr;
	int status = EXIT_SUCCESS;

	assert(!setvbuf(stdout, nullptr, _IONBF, 0));
	for (size_t i = 0; i != param->pathc; i++) {
		if (!strcmp(param->pathv[i], "-")) {
			dump(stdin);
			continue;
		}
		stream = fopen(param->pathv[i], "r");
		if (stream == nullptr) {
			fprintf(stderr, "%s: '%s': %s\n", param->argv0, param->pathv[i], strerror(errno));
			status = EXIT_FAILURE;
			continue;
		}
		dump(stream);
		assert(!fclose(stream));
	}
	if (param->pathc == 0)
		dump(stdin);

	return status;
}

int
main(int argc, char **argv)
{
	Param param = {0};

	if (!parse_arguments(&param, argc, argv))
		return EXIT_FAILURE;

	return run(&param);
}
