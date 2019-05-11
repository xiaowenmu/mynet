#ifndef REACTORTHREAD_H
#define REACTORTHREAD_H


#include"include/Noncopyable.h"
#include"include/Mutex.h"
#include"include/CondNoMutex.h"

namespace mynet{

class Reactor;

class ReactorThread:public Noncopyable{
	
public:
	ReactorThread(int num);
	~ReactorThread();

	Reactor* startLoop();

private:
	
	void threadFunc();
	Thread thread;
	Reactor *reactor;
	Mutex mutex;
	CondNoMutex cond;
	
	
	
};


}





#endif