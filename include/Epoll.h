#ifndef EPOLL_H
#define EPOLL_H

#include<map>
#include<vector>
#include"include/MacroDefine.h"
//#include"include/Handler.h"
#include<sys/epoll.h>
#include"include/Noncopyable.h"

namespace mynet{

class Handler;	


class Epoll:public Noncopyable{
	
public:
	typedef std::vector<struct epoll_event> eventList;
	typedef std::vector<Handler *> activeHandlerList;
	Epoll();
	~Epoll();
	
	
	void poll(int timeout,activeHandlerList *);
	void updateHandler(Handler *);//通过epoll修改，如ADD还是DEL
	void removeHandler(Handler *);//在MAP和epoll_ctl中删除该Handler（如果在epoll_ctl已添加过时就DEL，如果没有在epoll中就不做处理）
	
private:
	
	void fillActiveEventList(int,activeHandlerList *);
	void update(Handler *,int);
	int resizeTimes;
	static const int eventsSize;//events的大小
	static const int resizeTimeMax;  //当events大小大于1024时就分配1.5倍的空间，当小于1024时就分配2倍的空间
	const int epollfd;
	std::map<int,Handler *> handlerMap;//保存epoll监控的事件，考虑是否可以用哈希表
	eventList events;//保存epoll中返回的事件
	
	
	
	
	
};
	
	
}



#endif
