/* -*- mode:linux-c -*- */

/**
 * This demonstrates running an empty libev loop.  Nothing happens!
 */

#include <ev.h>
#include "macros.h"

int main(int argc, char ** argv)
{
	struct ev_loop * loop = EV_DEFAULT;
	printf(__FMT__ "ev_run(loop, 0) -> %d\n", __OUT__, ev_run(loop, 0));
	exit(EXIT_SUCCESS);
}
