#include"include/Reactor.h"
#include"include/Epoll.h"
#include"include/Handler.h"
#include"include/MacroDefine.h"
#include<sys/eventfd.h>
#include<unistd.h>
#include"include/ScopeMutex.h"

#include<iostream>


namespace mynet{
	
	int createEventFd(){
		int fd = eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
		ERRRET(fd < 0);
		return fd;
		
	}
	
	Reactor::Reactor():epoll(new Epoll()),wakeupFd(createEventFd()),wakeupHandler(new Handler(this,wakeupFd)),stop(false),threadId(Thread::gettid()),callingLoopFunc(false){
		
		wakeupHandler->setReadCallBack(std::bind(&Reactor::readWakeupFd,this));
		wakeupHandler->enableRead();
		
	}
	Reactor::~Reactor(){
		wakeupHandler->disableAll();
		wakeupHandler->removeSelf();
		close(wakeupFd);
	}
	
	
	void Reactor::updateHandler(Handler *handler){
		epoll->updateHandler(handler);
	}
	void Reactor::removeHandler(Handler *handler){
		epoll->removeHandler(handler);
	}
	void Reactor::loop(int ms){
		
		while(!stop){
			
			activeList.clear();
			epoll->poll(ms,&activeList);
			for(auto handler : activeList){
				handler->handleEvent();
			}
			handleLoopFunc();
			
		}
		
	}
	
	void Reactor::stopLoop(){//在别的线程执行的
		assert(stop == false);
		stop = true;
		wakeupReactor();
	}
	
	
	void Reactor::wakeupReactor(){
		//assert(stop == false);
		uint64_t one = 1;
		ssize_t n = write(wakeupFd, &one, sizeof(one));
		ERRRET(n != sizeof(uint64_t));
	}
	
	
	void Reactor::readWakeupFd(){
		uint64_t one = 1;
		ssize_t n = read(wakeupFd,&one,sizeof(one));	
		ERRRET(n != sizeof(uint64_t));
		
	}
	
	
	void Reactor::runInLoop(LoopFunc func){//处理比较紧急的事件，比如有新的文件描述符到来
		if(isInReactorThread()){
			
			func();
		}
		else{
			{
				ScopeMutex lock(mutex);
				loopFuncList.push_back(std::move(func));
			}
			wakeupReactor();
		}
		
	}
	void Reactor::queueInLoop(LoopFunc func){//处理不紧急的事件，比如关闭文件描述符
		{
			ScopeMutex lock(mutex);
			loopFuncList.push_back(std::move(func));
		}
		if(!isInReactorThread() || callingLoopFunc)
			wakeupReactor();
		
	}
	
	void Reactor::handleLoopFunc(){
		std::vector<LoopFunc> loopFunc;
		callingLoopFunc = true;
		{
			ScopeMutex lock(mutex);
			loopFunc.swap(loopFuncList);
		}
		for(const LoopFunc &func : loopFunc)
			func();
		callingLoopFunc = false;
		
	}
	void Reactor::printAllHandler(){
		epoll->printAllHandler();
	}
	
	
}
