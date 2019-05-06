#include"include/Condition.h"
#include<errno.h>
#include<time.h>
#include<stdint.h>


//超时返回false，返回true证明等到了条件
bool mynet::Condition::timedWait(int sec){
	struct timespec waitTime;
	ERRRET(clock_gettime(CLOCK_MONOTONIC,&waitTime));
	const int64_t oneSec = 1000000000;
	int64_t nanSec = sec * oneSec;
	waitTime.tv_sec += static_cast<time_t>((nanSec + waitTime.tv_nsec) / oneSec);
	waitTime.tv_nsec += static_cast<long>((nanSec + waitTime.tv_nsec) % oneSec);
	return !(ETIMEDOUT == pthread_cond_timedwait(&cond,mutex.getMutex(),&waitTime));
	
	
}