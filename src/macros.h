/* -*- mode:linux-c -*- */
#ifndef ABOUT_LIBEV_MACROS_H
#define ABOUT_LIBEV_MACROS_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __FMT__
#undef __FMT__
#endif
#ifdef __OUT__
#undef __OUT__
#endif
#define __FMT__ "%5d: "
#define __OUT__ __LINE__

#ifndef offsetof
#define offsetof(TYPE, FLD) ((size_t)(&(((TYPE*)0)->FLD)))
#endif

#ifndef containerof
#define containerof(PTR, TYPE, FLD) ((TYPE*)(((size_t)PTR)-offsetof(TYPE,FLD)))
#endif

#define Assert(X)						\
	do {							\
		if (!(X)) {					\
			printf(__FMT__ "Assert(%s) failed\n",	\
				__OUT__, #X);			\
			exit(EXIT_FAILURE);			\
		}						\
	} while (0)

#define Test(X)								\
	do {								\
		if (X) {						\
			printf(__FMT__ "ok   %s\n", __OUT__, #X);	\
		} else {						\
			printf(__FMT__ "FAIL %s\n", __OUT__, #X);	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0)

#define Is(X)								\
	do {								\
		if (X) {						\
			printf(__FMT__ "yes  %s\n", __OUT__, #X);	\
		} else {						\
			printf(__FMT__ "no   %s\n", __OUT__, #X);	\
		}							\
	} while (0)

#define Demo(X)					\
	do {					\
		printf("\n");			\
		printf("%s\n", #X);		\
		printf("\n");			\
		X;				\
	} while (0)

/**
 * @return (double)seconds from an ev_tstamp.
 */
static inline double to_seconds(ev_tstamp tstamp)
{
	return (double)tstamp;
}

#endif
