/* -*- mode:linux-c -*- */

/**
 * About ev_periodic.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <ev.h>

#include "macros.h"

struct client
{
	struct ev_periodic periodic;
	double expected;
	double alpha;
};

static void on_periodic(struct ev_loop * loop, struct ev_periodic * periodic,
			int revents)
{
	struct client * client = containerof(periodic, struct client,
					periodic);
	double lower = client->expected - client->alpha;
	double upper = client->expected + client->alpha;
	double got = ev_now(loop);
	Test(lower <= got && got <= upper);
	Assert(revents == EV_PERIODIC);
}

int main(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct client A;

	A.expected = to_seconds(ev_now(loop)) + 1.12;
	A.alpha = 0.01;
	ev_periodic_init(&A.periodic, on_periodic,
			to_tstamp(A.expected), 0, 0);
	ev_periodic_start(loop, &A.periodic);
	ev_run(loop, 0);

	exit(EXIT_SUCCESS);
}
