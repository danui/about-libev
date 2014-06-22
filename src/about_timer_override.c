/* -*- mode:linux-c -*- */

/**
 * Explores the question of overriding timers.
 */

#include <ev.h>
#include "macros.h"

static int verbose = 0;

struct task
{
	struct ev_timer timer;
	int rank;
};

static const double T = 0.01;
#define N  10
static int order[N];
static struct task tasks[N];
static int n = 0;

#define dump_order() __dump_order(__LINE__)
static void __dump_order(int lineno)
{
	int i;
	printf(__FMT__ "dump_order() called from line %d\n", __OUT__, lineno);
	printf(__FMT__ "  n is %d\n", __OUT__, n);
	for (i=0; i < n; i+=1)
		printf(__FMT__ "  order[%d] is %d\n", __OUT__, i, order[i]);
}

static void register_completition(struct task * task)
{
	Assert(n < N);
	order[n] = task->rank;
	n += 1;
}

static void on_timeout(struct ev_loop * loop, struct ev_timer * timer,
		int revents)
{
	struct task * task = containerof(timer, struct task, timer);
	(void)revents;		/* unused */
	register_completition(task);
	ev_timer_stop(loop, timer);
}

static void on_timeout_stop_next(struct ev_loop * loop,
				struct ev_timer * timer, int revents)
{
	struct task * task = containerof(timer, struct task, timer);
	struct task * next;
	(void)revents;		/* unused */
	register_completition(task);
	Assert(task->rank+1 < N);
	next = &tasks[task->rank+1];
	ev_timer_stop(loop, &next->timer);
}

static void on_timeout_delay_next(struct ev_loop * loop,
				struct ev_timer * timer, int revents)
{
	struct task * task = containerof(timer, struct task, timer);
	struct task * next;
	(void)revents;		/* unused */
	register_completition(task);
	Assert(task->rank+1 < N);
	next = &tasks[task->rank+1];
	/*
	 * NOTE: must stop timer before it can be safely reinitialized
	 * and started.
	 */
	ev_timer_stop(loop, &next->timer);
	ev_timer_init(&next->timer, on_timeout, T*N, 0);
	ev_timer_start(loop, &next->timer);
}

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	int i;
	/* Initialize task ranks */
	for (i=0; i<N; ++i) {
		tasks[i].rank = i;
	}

	/* Straight timeout order */
	n = 0;
	for (i=0; i<N; ++i) {
		ev_timer_init(&tasks[i].timer, on_timeout, T*i, 0);
		ev_timer_start(loop, &tasks[i].timer);
	}
	ev_run(loop, 0);
	Test(n == N);
	if (verbose)
		dump_order();
	for (i=0; i<N; ++i) {
		Assert(order[i] == i);
	}

	/* Tasks 0, 1, 2 are scheduled.  But task 0 would stop task
	 * 1.  Expected order is 0, 2*/
	n = 0;
	for (i=0; i<3; ++i) {
		if (i == 0)
			ev_timer_init(&tasks[i].timer, on_timeout_stop_next,
				T*i, 0);
		else
			ev_timer_init(&tasks[i].timer, on_timeout, T*i, 0);
		ev_timer_start(loop, &tasks[i].timer);
	}
	ev_run(loop, 0);
	if (verbose)
		dump_order();
	Test(n == 2);
	Test(order[0] == 0);
	Test(order[1] == 2);

	/* Tasks 0, 1, 2 are scheduled. But task 0 would reschedule 1 to
	 * the end.  Expected order is 0, 2, 1*/
	n = 0;
	for (i=0; i<3; ++i) {
		if (i == 0)
			ev_timer_init(&tasks[i].timer, on_timeout_delay_next,
				T*i, 0);
		else
			ev_timer_init(&tasks[i].timer, on_timeout, T*i, 0);
		ev_timer_start(loop, &tasks[i].timer);
	}
	ev_run(loop, 0);
	if (verbose)
		dump_order();
	Test(n == 3);
	Test(order[0] == 0);
	Test(order[1] == 2);
	Test(order[2] == 1);

	exit(EXIT_SUCCESS);
}
