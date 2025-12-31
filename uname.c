#include <sys/utsname.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum {
	ExitSuccess,
	ExitFailure,
};

typedef struct Param {
	char *argv0;
	bool a;
	bool m;
	bool n;
	bool r;
	bool s;
	bool v;
} Param;

static bool
parse_arguments(Param *param, int argc, char **argv)
{
	int opt = 0;

	assert(argc > 0);
	assert(argv != nullptr);
	param->argv0 = argv[0];

	while ((opt = getopt(argc, argv, "amnrsv")) >= 0) {
		switch (opt) {
		case 'a':
			param->a = true;
			break;
		case 'm':
			param->m = true;
			break;
		case 'n':
			param->n = true;
			break;
		case 'r':
			param->r = true;
			break;
		case 's':
			param->s = true;
			break;
		case 'v':
			param->v = true;
			break;
		case '?':
			fprintf(stderr, "usage: %s [-amnrsv]\n", argv[0]);
			return false;
		}
	}

	return true;
}

int
main(int argc, char **argv)
{
	Param param = {0};
	struct utsname utsname = {0};
	int count = 0;

	if (!parse_arguments(&param, argc, argv))
		return ExitFailure;

	if (uname(&utsname) < 0) {
		fprintf(stderr, "%s: %s\n", param.argv0, strerror(errno));
		return ExitFailure;
	}

	if (param.a) {
		param.m = true;
		param.n = true;
		param.r = true;
		param.s = true;
		param.v = true;		
	}

	count += param.m;
	count += param.n;
	count += param.r;
	count += param.s;
	count += param.v;

	if (count == 0) {
		param.s = true;
		count = 1;
	}
	if (param.s) {
		printf("%s", utsname.sysname);
		if (--count)
			putchar(' ');
	}
	if (param.n) {
		printf("%s", utsname.nodename);
		if (--count)
			putchar(' ');
	}
	if (param.r) {
		printf("%s", utsname.release);
		if (--count)
			putchar(' ');
	}
	if (param.v) {
		printf("%s", utsname.version);
		if (--count)
			putchar(' ');
	}
	if (param.m)
		printf("%s", utsname.machine);
	putchar('\n');

	return ExitSuccess;
}
