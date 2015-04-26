#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <time.h>
#include <inttypes.h>

uint64_t GetTimeStamp();
void PrintTime(uint64_t t);
void PrintTimeDiff(uint64_t t1, uint64_t t2);

#endif