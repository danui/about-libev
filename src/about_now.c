/* -*- mode:linux-c -*- */

/**
 * About ev_now().
 *
 * The concept of now as I expect it is that each iteration of an event
 * loop has a 'now' that is constant for the duration of that iteration.
 * In this about we explore what is before a loop runs, during an
 * iteration or two, and after the loop terminates.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <ev.h>

#include "macros.h"

static double walltime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}

struct client
{
	struct ev_timer timer;
	ev_tstamp tstamp;
	double wtime;
};

static void on_timeout(struct ev_loop * loop, struct ev_timer * timer, int revents)
{
	struct client * client = containerof(timer, struct client, timer);
	(void)revents;
	client->tstamp = ev_now(loop);
	client->wtime = walltime();
	usleep(1000);
}

int main(void)
{
	struct ev_loop * loop;
	struct ev_loop * another_loop;
	ev_tstamp t0, t1;
	struct client A, B;

	/*
	 * ev_now is the walltime for an iteration and so should not
	 * vary if there were no iterations.
	 */
	loop = EV_DEFAULT;
	t0 = ev_now(loop);
	usleep(200000);		/* 200 milliseconds */
	t1 = ev_now(loop);
	Test(t0 == t1);

	/*
	 * Does EV_DEFAULT allocate a new loop each time?
	 */
	another_loop = EV_DEFAULT;
	Is(loop == another_loop); /* ans: yes */
	if (loop != another_loop)
		Is(ev_now(loop) == ev_now(another_loop));

	/*
	 * different callbacks that trigger during the same iteration
	 * should witness the same value for ev_now.
	 */
	ev_timer_init(&A.timer, on_timeout, 0, 0);
	ev_timer_init(&B.timer, on_timeout, 0, 0);
	ev_timer_start(loop, &A.timer);
	ev_timer_start(loop, &B.timer);
	t0 = ev_now(loop);
	ev_run(loop, 0);
	t1 = ev_now(loop);

	/* The following must be true */
	Test(t0 <= A.tstamp);
	Test(A.tstamp == B.tstamp);
	Test(B.tstamp <= t1);

	/* The following may or may not be true depending on when now is
	 * updated.  */
	Is(t0 < t1);
	Is(t0 < A.tstamp);
	Is(B.tstamp < t1);
	Is(B.tstamp == t1);

	printf(__FMT__ "Client A tstamp: %lf wtime: %lf\n", __OUT__,
		to_seconds(A.tstamp), A.wtime);
	printf(__FMT__ "Client B tstamp: %lf wtime: %lf\n", __OUT__,
		to_seconds(B.tstamp), B.wtime);

	/*
	 * It is observed that ev_now after exiting the loop is equal to
	 * the ev_now of the last loop iteration.  I.e. ev_now is not
	 * updated when exiting the loop.
	 */
	exit(EXIT_SUCCESS);
}
