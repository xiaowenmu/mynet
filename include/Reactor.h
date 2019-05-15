#ifndef REACTOR_H
#define REACTOR_H

#include<vector>
#include<memory>
#include<functional>
#include"include/MacroDefine.h"
#include"include/Noncopyable.h"
#include"include/Mutex.h"
#include"include/Thread.h"

namespace mynet{

class Handler;
class Epoll;


class Reactor:public Noncopyable{
	
public:
	typedef std::vector<Handler *> activeHandlerList;
	typedef std::function<void ()> LoopFunc;
	Reactor();
	~Reactor();

	void updateHandler(Handler *);//调用epoll中的updateHandler,作用：根据Handler中的action标记把该Handler添加进epoll或从epoll中删除
	void removeHandler(Handler *);//调用epoll中的removeHandler,作用：把Handler从epoll的map中删除，并且移出epoll
	void loop(int ms);//
	void wakeupReactor();
	void readWakeupFd();
	void stopLoop();
	void runInLoop(LoopFunc);
	void queueInLoop(LoopFunc);
	bool isInReactorThread(){ return threadId == Thread::gettid();}

	void printAllHandler();
		
	

private:
	void handleLoopFunc();
	std::unique_ptr<Epoll> epoll;
	activeHandlerList activeList;
	int wakeupFd;
	pid_t threadId;
	std::unique_ptr<Handler> wakeupHandler;
	bool stop;
	bool callingLoopFunc;
	mutable Mutex mutex;
	std::vector<LoopFunc> loopFuncList;
	
	
};

}


#endif
