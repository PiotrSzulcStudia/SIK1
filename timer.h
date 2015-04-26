#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

uint64_t GetTimeStamp();
void PrintTimeDiff(uint64_t t1, uint64_t t2);

/* output to stderr */
void PrintTimeStamp(char *action, uint64_t t);
#endif