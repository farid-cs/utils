#include <assert.h>
#include <errno.h>
#include <fcntl.h>
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
	assert(argv);
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
write_all(int fd, const unsigned char *buf, size_t size)
{
	ssize_t nwrite;

	while (size > 0 && (nwrite = write(fd, buf, size)) > 0) {
		buf += (size_t)nwrite;
		size -= (size_t)nwrite;
	}
	assert(nwrite >= 0);
}

static void
handle_fd(int fd)
{
	unsigned char buf[BUFSIZ];
	ssize_t nread;

	while ((nread = read(fd, buf, BUFSIZ)) > 0)
		write_all(STDOUT_FILENO, buf, (size_t)nread);

	assert(nread == 0);
}

static bool
handle_file(const char *path)
{
	int fd = open(path, O_RDONLY);

	if (fd < 0)
		return false;

	handle_fd(fd);
	assert(close(fd) >= 0);

	return true;
}

int
main(int argc, char **argv)
{
	Param param = {0};
	int status = EXIT_SUCCESS;

	if (!parse_arguments(&param, argc, argv))
		return EXIT_FAILURE;

	for (size_t i = 0; i != param.pathc; i++) {
		if (!strcmp(param.pathv[i], "-")) {
			handle_fd(STDIN_FILENO);
		} else if (!handle_file(param.pathv[i])) {
			fprintf(stderr, "%s: '%s': %s\n", param.argv0, param.pathv[i], strerror(errno));
			status = EXIT_FAILURE;
		}
	}
	if (param.pathc == 0)
		handle_fd(STDIN_FILENO);

	return status;
}
