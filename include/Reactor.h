#ifndef REACTOR_H
#define REACTOR_H

#incldue"include/Handler.h"
#include"include/Epoll.h"


class Reactor:public Noncopyable{
	typedef std::vector<Handler *> activeHandlerList;
public:

	Reactor();
	~Reactor();

	void updateHandler(Handler *);//调用epoll中的updateHandler,作用：根据Handler中的action标记把该Handler添加进epoll或从epoll中删除
	void removeHandler(Handler *);//调用epoll中的removeHandler,作用：把Handler从epoll的map中删除，并且移出epoll
	void loop();//
	void wakeupReactor();
	void readWakeupFd();


private:
	Epoll epoll;
	activeHandlerList activeList;
	int wakeupFd;
	Handler wakeupHandler;
	
};




#endif