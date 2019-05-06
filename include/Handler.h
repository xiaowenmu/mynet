#ifndef HANDLER_H
#define HANDLER_H


#include"include/MacroDefine.h"

class Handler：public Noncopyable{

public:
	static const int New     = -1; //代表没有加入过epoll的描述符
	static const int Modify  = 0;  //代表需要修改epoll的动作,修改关注的事情或者在epoll中删除该事件,Modify && noFocus == 在epoll中删除
	static const int Deleted = 1;  //代表先加入了epoll后被删除的,但是在map中还存在（map保存在epoll中的，或者在epoll中被删除的）
	static const int Added   = 2;  //代表已经在epoll中的
	
	static const int noAttention = 0; //代表当前没有所关注的事情，可以在epoll中对其进行修改或删除
	
	
	explicit Handler();
	~Handler();
	int getfd() const{ return fd; }
	void setHappened(int event){ happened = event; }
	bool noFocus(){ return focus == noAttention; }
	void setAction(int act){ action = act; }
	int getFocus(){ return focus; }
	int getAction(){ return action; }
	

private:
	const int fd;
	int action;  //给epoll_ctl使用的，如New还是Modify
	int happened;//保存epoll返回时发生的事件，是read还是write等
	int focus;   //表示关注读事件还是写事件还是都不关注，如果都不关注就可以在epoll中删除掉了，并把标志设为Deleted。如focus == noAttention 代表可以从handlerMap中删除
};



#endif