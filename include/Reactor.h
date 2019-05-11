#ifndef REACTOR_H
#define REACTOR_H

#include<vector>
#include<memory>
#include"include/MacroDefine.h"
#include"include/Noncopyable.h"

namespace mynet{

class Handler;
class Epoll;


class Reactor:public Noncopyable{
	
public:
	typedef std::vector<Handler *> activeHandlerList;
	Reactor();
	~Reactor();

	void updateHandler(Handler *);//调用epoll中的updateHandler,作用：根据Handler中的action标记把该Handler添加进epoll或从epoll中删除
	void removeHandler(Handler *);//调用epoll中的removeHandler,作用：把Handler从epoll的map中删除，并且移出epoll
	void loop(int ms);//
	void wakeupReactor();
	void readWakeupFd();
	void stopLoop();


private:
	std::unique_ptr<Epoll> epoll;
	activeHandlerList activeList;
	int wakeupFd;
	std::unique_ptr<Handler> wakeupHandler;
	bool stop;
	
};

}


#endif
