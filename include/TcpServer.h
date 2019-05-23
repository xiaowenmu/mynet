#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<sys/types.h>
#include<sys/socket.h>
#include<memory>
#include<functional>
#include"include/Noncopyable.h"
#include"include/ReactorThreadPool.h"
#include"include/Connection.h"

namespace mynet{
	
class Reactor;
class Handler;
class Accept;
class Buffer;
	
class TcpServer:public Noncopyable{
	
public:
	typedef std::function<void (const ConnectionPtr &con,Buffer *buffer)> MessageCallBack;
	
	TcpServer(int num,struct sockaddr *addr);
	~TcpServer();
	
	
	//void setReadCallBack(readCallBack &readFunc);
	//void setWriteCallBack(writeCallBack &writeFunc);
	//void setCloseCallBack(closeCallBack &closeFunc);
	//void setErrCallBack(errCallBack &errFunc);
	
	void newConnection(int);
	void start();

	void printAllHandler();

	void setMessageCallBack(MessageCallBack fun){
		messageFunc = std::move(fun);
	}	

	void stop(){
		reactorPool->stopAll();
	}
	
private:
	//void createHandler(Reactor *reac,int fd);
	//void readFunc(Handler *handler);
	std::unique_ptr<ReactorThreadPool> reactorPool;
	std::unique_ptr<Accept> serverAccept;
	Reactor *mainReactor;
	bool started;
	MessageCallBack messageFunc;
	
	//readCallBack &readFunc;
	//writeCallBack &writeFunc;
	//closeCallBack &closeBack;
	//errCallBack &errFunc;
	
	
};
	
	
	
	
	
	
	
	
	
	
}







#endif
