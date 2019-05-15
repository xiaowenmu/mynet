#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<sys/types.h>
#include<sys/socket.h>
#include<memory>
#include"include/Noncopyable.h"
#include"include/ReactorThreadPool.h"

namespace mynet{
	
class Reactor;
class Handler;
class Accept;
	
class TcpServer:public Noncopyable{
	
public:
	
	
	TcpServer(int num,struct sockaddr *addr);
	~TcpServer();
	
	
	//void setReadCallBack(readCallBack &readFunc);
	//void setWriteCallBack(writeCallBack &writeFunc);
	//void setCloseCallBack(closeCallBack &closeFunc);
	//void setErrCallBack(errCallBack &errFunc);
	
	void newConnection(int);
	void start();

	void printAllHandler();
	
private:
	void createHandler(Reactor *reac,int fd);
	void readFunc(Handler *handler);
	std::unique_ptr<ReactorThreadPool> reactorPool;
	std::unique_ptr<Accept> serverAccept;
	Reactor *mainReactor;
	bool started;
	//readCallBack &readFunc;
	//writeCallBack &writeFunc;
	//closeCallBack &closeBack;
	//errCallBack &errFunc;
	
	
};
	
	
	
	
	
	
	
	
	
	
}







#endif
