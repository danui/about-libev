/* -*- mode:linux-c -*- */

/**
 * This demonstrates use of ev_timer.
 */

#include <ev.h>
#include "macros.h"

static int verbose = 1;

static void on_timeout(struct ev_loop * loop, ev_timer * w, int revents)
{
	static int count = 0;

	if (verbose) {
		printf(__FMT__ "count: %d\n", __OUT__, count);
		printf(__FMT__ "  address of loop is %p\n", __OUT__,
			(void*)loop);
		printf(__FMT__ "  address of w is %p\n", __OUT__, (void*)w);
		printf(__FMT__ "  revents: 0x%X\n", __OUT__,
			(unsigned)revents);
	}
	Assert(revents & EV_TIMER);
	count += 1;
	if (count > 3) {
		ev_timer_stop(loop, w);

		/* NOTE: ev_break is not required because loop stops
		 * when there is nothing left to do. */
		//ev_break(loop, EVBREAK_ALL);
	}
}

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct ev_timer timer;
	if (verbose) {
		printf(__FMT__ "address of loop is %p\n", __OUT__,
			(void*)loop);
		printf(__FMT__ "address of &timer is %p\n", __OUT__,
			(void*)&timer);
	}
	ev_timer_init(&timer, on_timeout, .1, 0.1);
	ev_timer_start(loop, &timer);
	Test(0 == ev_run(loop, 0));
	exit(EXIT_SUCCESS);
}
