#include <sys/utsname.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Param {
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

static int
run(Param param)
{
	struct utsname utsname = {0};
	int count = 0;

	assert(uname(&utsname) >= 0);

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

	return EXIT_SUCCESS;
}

int
main(int argc, char **argv)
{
	Param param = {0};

	if (!parse_arguments(&param, argc, argv))
		return EXIT_FAILURE;

	return run(param);
}
