#ifndef TIMER_H
#define TIMER_H
#include <sys/timerfd.h>
#include<functional>
#include<time.h>

namespace mynet{
	
class Timer{
	
public:
	typedef std::function<void ()> Fun;
	Timer(struct timespec nextInter,struct timespec inter,Fun call,bool abs = false);
	~Timer();
	struct itimerspec getTime(); //获取还有多久定时器到期
	void setTime(itimerspec &newTime,itimerspec &oldTime);//重新设置定时时间
	void runTimer();
	
	
	
private:
	void initialize();
	struct timespec nextInterrupt; //第一次定时时间为多久
	struct timespec interval; //定时时间间隔
	//int nextInterrupt;
	Fun callBack; //超时后要调用的函数
	bool absTime;//定时设置的时间是否是绝对时间,目前只能设置为相对时间
	int fd;//后面可以改成reactor中的持有fd的结构体
	bool started;//保证runTimer只执行一次
	

	
};
	
	
	
}








#endif