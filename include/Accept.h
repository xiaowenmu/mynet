#ifndef ACCEPT_H
#define ACCEPT_H
#include"include/Noncopyable.h"
#include<sys/types.h>
#include<sys/socket.h>
#include"include/Handler.h"

namespace mynet{
	
	
class Accept:public Noncopyable{
public:
	typedef std::function<void (int)> newConnectCallBack;
	Accept(Reactor *reac,struct sockaddr*);
	~Accept();
	void handleRead();
	void setNewConnectCallBack(newConnectCallBack func){ newCallBack = std::move(func); }
	void listen();
	Accept* getPointer();
private:
	Reactor *reactor;
	int acceptFd;
	Handler acceptHandler;
	newConnectCallBack newCallBack;
	bool listenning;
	int times;
	
	
	
};
	
	
	
	
}






#endif
