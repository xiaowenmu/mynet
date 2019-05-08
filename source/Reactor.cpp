#include"include/Reactor.h"


namespace mynet{
	
	int createEventFd(){
		int fd = eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
		ERRRET(fd);
		return fd;
		
	}
	
	Reactor::Reactor():wakeupFd(createEventFd()),wakeupHandler(this,wakeupFd){
		
		wakeupHandler.setReadCallBack(std::bind(&Reactor::readWakeupFd,this));
		wakeupHandler.enableRead();
		
	}
	Reactor::~Reactor(){
		wakeupHandler.disableAll();
		wakeupHandler.removeHandler();
		close(wakeupFd);
	}
	
	
	void Reactor::updateHandler(Handler *handler){
		epoll.updateHandler(handler);
	}
	void Reactor::removeHandler(Handler *handler){
		epoll.removeHandler(handler);
	}
	void Reactor::loop(int ms){
		
		while(1){
			
			activeList.clear();
			epoll.poll(ms,activeList);
			for(auto handler : activeList){
				handler->handleEvent();
			}
			
		}
		
	}
	
	void Reactor::wakeupReactor(){
		uint64_t one = 1;
		ssize_t n = write(wakeupFd, &one, sizeof(one));
		ERRRET(n != sizeof(uint64_t));
	}
	
	
	void Reactor::readWakeupFd(){
		uint64_t one;
		ssize_t n = read(wakeupFd,&one,sizeof(one));
		ERRRET(n != sizeof(uint64_t));
		
	}
	
	
	
}