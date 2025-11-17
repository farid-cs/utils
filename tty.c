#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
	const char *tty = ttyname(STDIN_FILENO);

	if (tty == nullptr) {
		assert(errno == ENOTTY);
		printf("not a tty\n");
		return 1;
	}
	printf("%s\n", tty);

	return EXIT_SUCCESS;
}
