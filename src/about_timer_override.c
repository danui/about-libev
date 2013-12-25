/* -*- mode:linux-c -*- */

/**
 * Explores the question of overriding timers.
 */

#include <ev.h>
#include "macros.h"

static struct ev_timer t1;
static struct ev_timer t2;
static struct ev_timer t3;
static int n1 = 0;
static int n2 = 0;
static int n3 = 0;

static void on_t1_cancel_t2(struct ev_loop * loop,
			struct ev_timer * timer, int revents)
{
	(void)revents;
	Assert(timer == &t1);
	n1 += 1;
	ev_timer_stop(loop, &t2);
}

static void on_t2_inc_n2(struct ev_loop * loop,
			struct ev_timer * timer, int revents)
{
	(void)loop;
	(void)timer;
	(void)revents;
	n2 += 1;
}

static void on_t3_stop(struct ev_loop * loop,
		struct ev_timer * timer, int revents)
{
	(void)timer;
	(void)revents;
	n3 += 1;
	ev_break(loop, EVBREAK_ALL);
}

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	ev_timer_init(&t1, on_t1_cancel_t2, .1, 0);
	ev_timer_init(&t2, on_t2_inc_n2, .2, 0);
	ev_timer_init(&t3, on_t3_stop, .3, 0);

	ev_timer_start(loop, &t1);
	ev_timer_start(loop, &t2);
	ev_timer_start(loop, &t3);

	Test(0 == ev_run(loop, 0));
	Test(n1 == 1);
	Test(n2 == 0);
	Test(n3 == 1);
	exit(EXIT_SUCCESS);
}
