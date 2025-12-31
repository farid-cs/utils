#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum {
	ExitSuccess,
	ExitNotTty,
	ExitFailure,
};

static char *
parse_arguments(int argc, char **argv)
{
	assert(argc > 0);
	assert(argv != nullptr);
	return argv[0];
}

int
main(int argc, char **argv)
{
	const char *argv0 = parse_arguments(argc, argv);
	const char *tty = ttyname(STDIN_FILENO);

	if (tty == nullptr) {
		if (errno != ENOTTY) {
			fprintf(stderr, "%s: %s\n", argv0, strerror(errno));
			return ExitFailure;
		}
		printf("not a tty\n");
		return ExitNotTty;
	}
	printf("%s\n", tty);

	return ExitSuccess;
}
