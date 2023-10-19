#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

#pragma region "Time"

enum {
  seconds_e = 1,
  milliseconds_e = 1000,
  microseconds_e = 1000000,
  nanoseconds_e = 1000000000,
};

typedef struct _timepoint_t
{
	#if defined(_WIN32)

	long c;
	#elif defined(__GNUC__)
	long s;
	long ns;

	#endif
} timepoint_t;

timepoint_t timenow()
{
	#if defined(_WIN32)
	timepoint_t tp;
	tp.c = clock();
	return tp;
	#elif defined(__GNUC__)
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	timepoint_t tp;
	tp.s = ts.tv_sec;
	tp.ns = ts.tv_nsec;
	return tp;
	#endif
}

double timeduration(timepoint_t end,
	timepoint_t start, double ratio)
{
	#if defined(_WIN32)
		double ndif = (double)(end.c - start.c) / (double)CLOCKS_PER_SEC;
		return ndif * ratio;
	#elif defined(__GNUC__)
	double t1 = (double)end.s + (double)end.ns / 1000000000.0;
	double t2 = (double)start.s + (double)start.ns / 1000000000.0;

	return (t1-t2) * ratio;
	#endif
}

void wait(double seconds) {
  timepoint_t s = timenow();
  timespec ts, tsr = {2000, 0};
  while (timeduration(timenow(), s, seconds_e) < seconds)
    {nanosleep(&ts, &tsr);}
}
void waitms(double ms) {
  timepoint_t s = timenow();
  timespec ts, tsr = {2000, 0};
  while (timeduration(timenow(), s, milliseconds_e) < ms)
    {nanosleep(&ts, &tsr);}
}

#pragma endregion "Time"