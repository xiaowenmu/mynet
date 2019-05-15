#include"include/ReactorThread.h"
#include"include/ScopeMutex.h"
#include<assert.h>
#include<functional>
#include"include/Reactor.h"




namespace mynet{
	
	ReactorThread::ReactorThread(int num):thread(std::bind(&ReactorThread::threadFunc,this),num),reactor(nullptr),mutex(),cond(mutex){
		
	}
	ReactorThread::~ReactorThread(){//待补充
		assert(thread.threadStarted());//不允许还没执行startLoop就结束
		while(reactor == nullptr);//这里采取等待reactor创建好，这种情况很少，并且等待很短一段时间就能满足条件
		
		reactor->stopLoop();
		reactor->wakeupReactor();
		thread.join();
		
	}
	
	
	Reactor* ReactorThread::startLoop(){
		assert(!thread.threadStarted());
		thread.run();
		{
			ScopeMutex lock(mutex);
			while(reactor == nullptr){
				cond.wait();
			}
			
		}
		return reactor;
		
	}
	
	
	void ReactorThread::threadFunc(){
		Reactor reac;
		{
			ScopeMutex lock(mutex);
			reactor = &reac;
			cond.notify();
			
		}
		
		reac.loop(-1);
		
		ScopeMutex lock(mutex);
		reactor = nullptr;
		
	}
	
	
	
}
