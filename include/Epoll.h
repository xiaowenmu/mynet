#ifndef EPOLL_H
#define EPOLL_H

#include<map>
#include<vector>
#include"include/MacroDefine.h"


namespace mynet{
	
class Epoll：public Noncopyable{
	
public:
	typedef std::vector<struct epoll_event> eventList;
	explicit Epoll();
	~Epoll();
	
	
	void poll(int timeout,eventList *);
	void updateHandler(Handler *);
	void removeHandler(Handler *);
	
private:

	void fillActiveEventList(int,activeHandlerList *);
	void update(Handler *,int);
	int resizeTimes;
	static const int eventsSize = 32;//events的大小
	static const int resizeTimeMax = 1024 / eventsSize;  //当events大小大于1024时就分配1.5倍的空间，当小于1024时就分配2倍的空间
	const int epollfd;
	std::map<int,handler *> handlerMap;//保存epoll监控的事件，考虑是否可以用哈希表
	eventList events;//保存epoll中返回的事件
	
	
	
	
	
};
	
	
}



#endif