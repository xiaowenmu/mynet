#include"include/ThreadPool.h"
#include<iostream>
#include<cstdlib>
#include<functional>
#include"include/Timer.h"
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
//#include <stdio.h>
#include <stdint.h>
using namespace std;
#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

void print_elapsed_time(void)
{
      static struct timespec start;
      struct timespec curr;
      static int first_call = 1;
      int secs, nsecs;

      if (first_call) {
          first_call = 0;
          if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
              handle_error("clock_gettime");
      }

      if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1)
          handle_error("clock_gettime");

          secs = curr.tv_sec - start.tv_sec;
          nsecs = curr.tv_nsec - start.tv_nsec;
          if (nsecs < 0) {
              secs--;
              nsecs += 1000000000;
          }
          printf("%d.%03d: \n", secs, (nsecs + 500000) / 1000000);
      }


int main(){

	struct timespec next;
	next.tv_sec = 3;
	next.tv_nsec = 400;
	struct timespec inter;
	inter.tv_sec = 6;
	inter.tv_nsec = 200;
	
	mynet::Timer timer(next,inter,print_elapsed_time);
	print_elapsed_time();
	timer.runTimer();
	

	while(1) {
		uint64_t exp;
		ssize_t s;
               s = read(timer.getFd(), &exp, sizeof(uint64_t));
               if (s < 0)
                   handle_error("read");

         
               print_elapsed_time();
              
           }

	
	while(1);	

}
