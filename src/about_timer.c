/* -*- mode:linux-c -*- */

/**
 * This demonstrates use of ev_timer.
 */

#include <ev.h>
#include "macros.h"

static void on_timeout(struct ev_loop * loop, ev_timer * w, int revents)
{
	static int count = 0;
	printf(__FMT__ "count: %d\n", __OUT__, count);
	printf(__FMT__ "address of loop is %p\n", __OUT__, (void*)loop);
	printf(__FMT__ "address of w is %p\n", __OUT__, (void*)w);
	count += 1;
	if (count > 3) {
		ev_timer_stop(loop, w);
		ev_break(loop, EVBREAK_ALL);
	}
}

int main(int argc, char ** argv)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct ev_timer timer;
	printf(__FMT__ "address of loop is %p\n", __OUT__, (void*)loop);
	printf(__FMT__ "address of &timer is %p\n", __OUT__, (void*)&timer);
	ev_timer_init(&timer, on_timeout, .1, 0.1);
	ev_timer_start(loop, &timer);
	printf(__FMT__ "ev_run -> %d\n", __OUT__, ev_run(loop, 0));
	exit(EXIT_SUCCESS);
}
