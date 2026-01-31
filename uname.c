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
	int opt;

	assert(argc > 0);
	assert(argv != nullptr);
	param->argv0 = argv[0];
	param->a = false;
	param->m = false;
	param->n = false;
	param->r = false;
	param->s = false;
	param->v = false;

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
	Param param;
	struct utsname utsname;
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

	if (param.s) {
		printf("%s", utsname.sysname);
		count += 1;
	}
	if (param.n) {
		if (count++)
			putchar(' ');
		printf("%s", utsname.nodename);
	}
	if (param.r) {
		if (count++)
			putchar(' ');
		printf("%s", utsname.release);
	}
	if (param.v) {
		if (count++)
			putchar(' ');
		printf("%s", utsname.version);
	}
	if (param.m) {
		if (count++)
			putchar(' ');
		printf("%s", utsname.machine);
	}
	if (count == 0)
		printf("%s", utsname.sysname);
	putchar('\n');

	return ExitSuccess;
}
