/* -*- mode:linux-c -*- */

/**
 * About how to use user context with libev.
 */

#include <ev.h>
#include "macros.h"

static int verbose = 1;

struct countdown_task
{
	int count;
	struct ev_timer timer;
};

static void countdown_tick(struct ev_loop * loop,
			struct ev_timer * timer, int revents)
{
	struct countdown_task * task;
	Assert(revents & EV_TIMER);
	task = containerof(timer, struct countdown_task, timer);
	if (task->count <= 0) {
		ev_timer_stop(loop, timer);
		ev_break(loop, EVBREAK_ALL);
	} else {
		if (verbose)
			printf(__FMT__ "tick, count: %d\n", __OUT__,
				task->count);
		task->count -= 1;
	}
}

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct countdown_task * task;
	Assert(NULL != (task = calloc(1, sizeof(struct countdown_task))));
	task->count = 3;
	ev_timer_init(&task->timer, countdown_tick, 0, 0.25);
	ev_timer_start(loop, &task->timer);
	ev_run(loop, 0);
	free(task);
	exit(EXIT_SUCCESS);
}
