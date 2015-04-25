#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <inttypes.h>

int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

int main(int argc, char **argv)
{
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Some code I am interested in measuring
  int i = 0;
  for ( i = 0; i <= 1000; i++)
    printf("a");

  clock_gettime(CLOCK_MONOTONIC, &end);

  uint64_t timeElapsed = timespecDiff(&end, &start);
  printf("\n %" PRId64 "ms\n", timeElapsed);
}