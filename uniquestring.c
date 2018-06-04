#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include "uniquestring.h"
typedef struct timeval Time_t;

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif
void time_now(Time_t *t)
{
	gettimeofday(t, NULL);
}

time_t time_valuesec(Time_t *t)
{
	return t->tv_sec;
}

int time_valueusec(Time_t *t)
{
	return t->tv_usec;
}

unsigned int random_uint()
{
	unsigned int ret, seed;
	unsigned int sec, usec;
	Time_t now;
	time_now(&now);
	sec = (unsigned int)time_valuesec(&now);
	usec = (unsigned int)time_valueusec(&now);
	seed = (((sec % 0xFFFF) << 16) | (usec % 0xFFFF));
	srand(seed);
	ret = rand();
	return ret;
}

int getuniquestring(const char *reference, char *unique_string, int size)
{
	Time_t now;
	time_now(&now);
#ifdef WIN32
	sprintf(unique_string, "%I64u%s", time_valuesec(&now), reference);
#else
	sprintf(unique_string, "%ld%s", time_valuesec(&now), reference);
#endif
	return 0;
}

#if 0
int main(void)
{
	char unique_string[128 + 1];
	memset(unique_string, 0, sizeof(unique_string));
	getuniquestring("5976baca7c5a97197300002f", unique_string, 128);
	printf("%s\n", unique_string);
}
#endif
