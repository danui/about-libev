/* -*- mode:linux-c -*- */

/**
 * This demonstrates use of ev_io
 *
 * A socketpair is created.  A writer task writes to one end, and a
 * reader task reads from the other.  Finally, test if memcmp on the
 * task buffers to verify.
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ev.h>
#include "macros.h"

struct task
{
	int fd;
	unsigned char * buf;
	size_t len;
	size_t offset;
	struct ev_io io;
	int verbose;
};

static void when_writable(struct ev_loop * loop, struct ev_io * io,
			int revents)
{
	struct task * task = containerof(io, struct task, io);
	ssize_t n;
	int err;

	if (task->verbose)
		printf(__FMT__
			"write fd: %d, buf: %p, len: %zu, offset: %zu\n",
			__OUT__, task->fd, (void*)task->buf, task->len,
			task->offset);
	n = write(task->fd, task->buf + task->offset,
		task->len - task->offset);
	if (n < 0) {
		err = errno;
		if ((err == EAGAIN) || (err == EWOULDBLOCK) ||
			(err == EINTR)) {
			/* do nothing, try again. */
		} else {
			printf(__FMT__ "write failed, %s\n", __OUT__,
				strerror(err));
			exit(EXIT_FAILURE);
		}
	} else if (n == 0) {
		printf(__FMT__ "wrote zero bytes\n", __OUT__);
		exit(EXIT_FAILURE);
	}

	if (task->verbose)
		printf(__FMT__ "wrote %zd bytes\n", __OUT__, n);
	task->offset += n;
	if (task->offset == task->len)
		ev_io_stop(loop, io);
}

static void when_readable(struct ev_loop * loop, struct ev_io * io,
			int revents)
{
	struct task * task = containerof(io, struct task, io);
	ssize_t n;
	int err;

	if (task->verbose)
		printf(__FMT__
			"read fd: %d, buf: %p, len: %zu, offset: %zu\n",
			__OUT__, task->fd, (void*)task->buf, task->len,
			task->offset);
	n = read(task->fd, task->buf + task->offset,
		task->len - task->offset);
	if (n < 0) {
		err = errno;
		if ((err == EAGAIN) || (err == EWOULDBLOCK) ||
			(err == EINTR)) {
			/* do nothing, try again. */
		} else {
			printf(__FMT__ "read failed, %s\n", __OUT__,
				strerror(err));
			exit(EXIT_FAILURE);
		}
	} else if (n == 0) {
		printf(__FMT__ "read zero bytes\n", __OUT__);
		exit(EXIT_FAILURE);
	}
	if (task->verbose)
		printf(__FMT__ "read %zd bytes\n", __OUT__, n);
	task->offset += n;
	if (task->offset == task->len)
		ev_io_stop(loop, io);
}

int main(int argc, char ** argv)
{
	const size_t N = 65536;
	int fds[2];
	struct ev_loop * loop = EV_DEFAULT;
	struct task A;
	struct task B;

	Assert(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
	/* For some reason the fds must be nonblocking. */
	Assert(0 == fcntl(fds[0], F_SETFL, O_NONBLOCK));
	Assert(0 == fcntl(fds[1], F_SETFL, O_NONBLOCK));

	/* Task A will be writer */
	A.fd = fds[0];
	Assert(NULL != (A.buf = malloc(N)));
	A.len = N;
	A.offset = 0;
	memset(A.buf, 0x11, A.len);
	ev_io_init(&A.io, when_writable, A.fd, EV_WRITE);
	ev_io_start(loop, &A.io);

	/* Task B will be reader */
	B.fd = fds[1];
	Assert(NULL != (B.buf = malloc(N)));
	B.len = N;
	B.offset = 0;
	memset(B.buf, 0x22, B.len);
	ev_io_init(&B.io, when_readable, B.fd, EV_READ);
	ev_io_set(&B.io, B.fd, EV_READ);
	ev_io_start(loop, &B.io);

	/* Verbose? */
	A.verbose = 0;
	B.verbose = 0;

	Test(0 != memcmp(A.buf, B.buf, N));
	Test(0 == ev_run(loop, 0));
	Test(0 == memcmp(A.buf, B.buf, N));

	exit(EXIT_SUCCESS);
}
