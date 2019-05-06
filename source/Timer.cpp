#include"include/Timer.h"
#include"include/MacroDefine.h"
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
//#include <stdio.h>
#include <stdint.h>
#include<iostream>


namespace mynet{
	
        void Timer::initialize(){
		
		started = false;
		
	}
	
	Timer::Timer(struct timespec nextInter,struct timespec inter,Fun call,bool abs):nextInterrupt(nextInter),interval(inter),callBack(std::move(call)),absTime(abs){
		fd = timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC);
		initialize();
	};
	
	
	
	Timer::~Timer(){
		close(fd);
	};
	
	
	
	
	
	
	
	
	struct itimerspec Timer::getTime(){
		struct itimerspec curr;
		ERRRET(timerfd_gettime(fd,&curr));
		return curr;
		
	}
	
	void Timer::runTimer(){  //保证runTimer只执行一次，如果调用间隔非常短的话，那么就有可能调用很多次，但是对定时时间影响不太大，所以不加锁了。
		
		if(started == false){
			struct itimerspec new_value;
			new_value.it_value = nextInterrupt;
			new_value.it_interval = interval;
			if(!absTime){
				ERRRET(timerfd_settime(fd, 0/*TFD_TIMER_ABSTIME*/, &new_value, NULL));
				
			}
			else{
				ERRRET(timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL));
			}			
			started = true;
		}
		return;
		
	}
	
	void Timer::setTime(itimerspec &new_value,itimerspec &old_value){
		if(!absTime){
			ERRRET(timerfd_settime(fd, 0/*TFD_TIMER_ABSTIME*/, &new_value, &old_value));
			
		}		
		else{
			ERRRET(timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, &old_value));
		}		
		return;
	}

	int Timer::getFd(){
		return fd;
	}
	
	
	
	
}
