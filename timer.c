#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#include "timer.h"

uint64_t GetTimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

void PrintTimeStamp(char *action, uint64_t t)
{
    fprintf(stderr, "Timestamp of %s: %" PRId64 " us\n", action,  t);
}

void PrintTimeDiff(uint64_t t1, uint64_t t2)
{
    printf("Time difference: %" PRId64 " us\n", t2 - t1);
}