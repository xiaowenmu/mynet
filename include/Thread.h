#ifndef THREAD_H
#define THREAD_H


#include"include/Condition.h"
#include<functional>
//#include<memory>
#include<sys/syscall.h>
#include <unistd.h>
#include<string>

namespace mynet{


//如果使用不当会不安全，先把功能实现再来改进
class Thread:public Noncopyable{

public:
	typedef std::function<void ()> ThreadFun;
	static pid_t gettid(){
		return static_cast<pid_t>(::syscall(SYS_gettid)); //getpid返回的tgid，不是pid（但进程中tgid = pid），是唯一的。pthread_self返回的是一个进程中线程的标识，相同进程中是唯一的，不同进程中的可以会相同。tid是由没和管理的不会有相同的
	}
	
	Thread(ThreadFun f,int i = 0);//name = threadi
	~Thread();
	void run();
	int join();
	bool threadStarted(){ return started; }

private:
	pid_t tid;
	Mutex threadMutex;
	pthread_t threadId;
	Condition condition;
	ThreadFun func;
	bool started;
	bool joined;
	std::string name;
	
};


}	



#endif
