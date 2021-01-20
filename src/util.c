/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

//#define LOGGING

void
die(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
#ifdef LOGGING
  FILE *log = fopen("/home/john/errlog.txt", "w+");
	vfprintf(log, fmt, ap);
#else
	vfprintf(stderr, fmt, ap);
#endif
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
#ifdef LOGGING
		fputc(' ', log);
    fputs(strerror(errno), log);
#else
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
#endif
	}

	exit(1);
}
