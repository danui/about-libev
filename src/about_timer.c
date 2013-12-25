/* -*- mode:linux-c -*- */
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

static void demo_timer(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct ev_timer timer;
	printf(__FMT__ "address of loop is %p\n", __OUT__, (void*)loop);
	printf(__FMT__ "address of &timer is %p\n", __OUT__, (void*)&timer);
	ev_timer_init(&timer, on_timeout, .1, 0.1);
	ev_timer_start(loop, &timer);
	printf(__FMT__ "ev_run -> %d\n", __OUT__, ev_run(loop, 0));
}

struct countdown_task
{
	int count;
	struct ev_timer timer;
};

static void countdown_tick(struct ev_loop * loop,
			struct ev_timer * timer, int revents)
{
	struct countdown_task * task;
	task = containerof(timer, struct countdown_task, timer);
	if (task->count <= 0) {
		ev_timer_stop(loop, timer);
		ev_break(loop, EVBREAK_ALL);
	} else {
		printf(__FMT__ "tick, count: %d\n", __OUT__, task->count);
		task->count -= 1;
	}
}

static void demo_countdown_task(void)
{
	struct ev_loop * loop = EV_DEFAULT;
	struct countdown_task * task;
	Assert(NULL != (task = calloc(1, sizeof(struct countdown_task))));
	task->count = 3;
	ev_timer_init(&task->timer, countdown_tick, 0, 0.25);
	ev_timer_start(loop, &task->timer);
	printf(__FMT__ "ev_run -> %d\n", __OUT__, ev_run(loop, 0));
	free(task);
}

int main(int argc, char ** argv)
{
	Demo(demo_timer());
	Demo(demo_countdown_task());
	printf("\n");
	return 0;
}
