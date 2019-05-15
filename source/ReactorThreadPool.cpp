#include"include/ReactorThreadPool.h"
#include<memory>
#include<assert.h>
#include"include/ReactorThread.h"
#include"include/Reactor.h"
namespace mynet{
	
	ReactorThreadPool::ReactorThreadPool(int num):threadNum(num),started(false),nextReactor(1){//编号为0的reactor是给accept用的
		assert(num >= 2);
		//startThreadPool();
		
	}
	ReactorThreadPool::~ReactorThreadPool(){
		//不需要销毁reactor，因为它是栈中对象
		
	}
	
	
	
	
	void ReactorThreadPool::startThreadPool(){
		assert(started == false);
		assert(threadNum != 0);
		started = true;
		for(int i = 0; i < threadNum; ++i){
			
			ReactorThread *p = new ReactorThread(i);
			reactorThreads.push_back(std::unique_ptr<ReactorThread>(p));
			allReactor.push_back(p->startLoop());
			
		}
		
		
	}
	
	Reactor *ReactorThreadPool::getNextReactor(){
		assert(started);
		Reactor *reac = allReactor[nextReactor];
		++nextReactor;
		if(nextReactor == allReactor.size())
			nextReactor = 1;
		
		return reac;
		
	}
	
	Reactor *ReactorThreadPool::getMainReactor(){
		return allReactor[0];
		
	}
	
	void ReactorThreadPool::printAllHandler(){
		for(int i = 0; i < allReactor.size(); ++i)
			allReactor[i]->printAllHandler();
	}

	
	
}
