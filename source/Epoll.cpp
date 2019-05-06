#include"include/Epoll.h"
#include"include/MacroDefine.h"
#include"include/Handler.h"
#include<errno.h>
#include<assert.h>
#include<sys/epoll.h>
namespace mynet{
	
	Epoll::Epoll():events(eventsSize){
		epollfd = epoll_create(1);//参数没有什么意义，只要大于0就可
		resizeTimes = 0;
	}
	Epoll::~Epoll(){
		close(epollfd);
	}
	
	void Epoll::poll(int ms,activeHandlerList *activeList){
		int eventNum = epoll_wait(epollfd,&*events.begin(),static_cast<int>(events.size()),ms);
		if(eventNum > 0){
			fillActiveEventList(eventNum,activeList);
			if(eventNum == events.size()){
				++resizeTimes;
				resizeTimes > resizeTimeMax ？events.resize(static_cast<size_t>(1.5*events.size()) : events.resize(static_cast<size_t>(2*events.size());
			}
		}
		else if(eventNum < 0){
			if(errno != EINTR)
				ERRRET(eventNum);
				
		}
		
		
	}
	
	
	void Epoll::fillActiveEventList(int nums,activeHandlerList *activeList){
		
		for(int i = 0; i < nums; ++i){
			Handler *cur = static_cast<Handler *>(events[i].data.ptr);
			int fd = cur->getfd();
			cur->setHappened(events[i].events);
			activeList.push_back(cur);	
		}
		
	}
	
	
	void Epoll::updateHandler(Handler *handler){//最多从epoll中删除掉Handler，不会再handlerMap中删掉
		
		int flag = handler->action;
		if(flag == Handler::New){//没有加入过epoll中的
			assert(handlerMap.find(handler->getfd()) == handlerMap.end());
			update(handler,EPOLL_CTL_ADD);
			handler->setAction(Handler::Added);
		}
		else if(flag == Handler::Deleted){//加入过后来被删除了的
			assert(handlerMap.find(handler->getfd()) != handlerMap.end());
			handler->setAction(Handler::Added);
			update(handler,EPOLL_CTL_ADD）;
			//handler->setAction(Handler::Added); //放前面还是后面后面再看
		}
		else if(flag == Handler::Modify){//加入过，但是需要修改关注的动作的
			assert(handlerMap.find(handler->getfd()) != handlerMap.end());
			if(handler->noFocus()){
				update(handler,EPOLL_CTL_DEL);
				handler->setAction(Handler::Deleted);
			}
			else{
				handler->setAction(Handler::Added);
				update(handler,EPOLL_CTL_MOD);
				//handler->setAction(Handler::Added);
			}
			
		}
		
		
		
	}
	
	void Epoll::update(Handler *handler,int operation){
		struct epoll_event event;
		memset(&event,0,sizeof(event));
		event.events = handler->getFocus();
		event.data.ptr = handler;
		int fd = handler->getfd();
		ERRRET(epoll_ctl(epollfd, operation, fd, &event));
		
		
	}
	
	void Epoll::removeHandler(Handler *handler){
		assert(handlerMap.find(handler->getfd()) != handlerMap.end());
		assert(handler->noFocus());//只要是noFocus就可以删除
		int fd = handler->getfd();
		handlerMap.erase(fd);
		int action = handler->getAction();
		if(action == Handler::Added)
			update(handler,EPOLL_CTL_DEL);
		
		handler->setAction(Handler::New);
	}
	
	
	
	
	
	
	
	
	
}