#include <assert.h>
#include <stdio.h>

static void
run(const char *expletive)
{
	if (!expletive)
		expletive = "y";
	while (true)
		printf("%s\n", expletive);
}

int
main(int argc, char **argv)
{
	const char *expletive = {};

	assert(argv);
	if (argc > 1)
		expletive = argv[1];

	run(expletive);
}
