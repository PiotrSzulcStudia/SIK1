#include "timer.h"

uint64_t GetTimeStamp()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}

void PrintTime(uint64_t t)
{
    printf("%" PRId64 " ms\n", t);
}

void PrintTimeDiff(uint64_t t1, uint64_t t2)
{
    printf("Time difference: %" PRId64 " ms\n", t2 - t1);
}