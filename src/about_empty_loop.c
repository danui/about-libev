/* -*- mode:linux-c -*- */

/**
 * This demonstrates running an empty libev loop.  Nothing happens!
 */

#include <ev.h>
#include "macros.h"

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	printf(__FMT__ "ev_run(loop, 0)...\n", __OUT__);
	ev_run(loop, 0);
	printf(__FMT__ "DONE\n", __OUT__);
	exit(EXIT_SUCCESS);
}
