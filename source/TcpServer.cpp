#include"include/TcpServer.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<memory>
#include<assert.h>
#include<unistd.h>
#include<fcntl.h>
#include"include/ReactorThreadPool.h"
#include"include/Accept.h"
#include"include/Reactor.h"




namespace mynet{
	
	TcpServer::TcpServer(int num,struct sockaddr *addr):reactorPool(new ReactorThreadPool(num)),started(false),mainReactor(nullptr){
		reactorPool->startThreadPool();
		mainReactor = reactorPool->getMainReactor();
		serverAccept.reset(new Accept(mainReactor,addr));
	}
	
	TcpServer::~TcpServer(){
			
		
	}
	
	
	void TcpServer::start(){
		assert(started == false);
		serverAccept->setNewConnectCallBack(std::bind(&TcpServer::newConnection,this,std::placeholders::_1));
		mainReactor->runInLoop(std::bind(&Accept::listen, serverAccept->getPointer()));
		
	}
	
	void TcpServer::newConnection(int fd){ //不用担心线程安全问题，只会在主reactor中调用
		int flag = fcntl(fd,F_GETFL,0);
		flag |= O_NONBLOCK;
		fcntl(fd,F_SETFL,flag);
		Reactor *nextReactor = reactorPool->getNextReactor();
		nextReactor->runInLoop(std::bind(&TcpServer::createHandler,this,nextReactor,fd)); 
		
		
		
		
	}
	
	void TcpServer::readFunc(Handler *handler){
		char buf[1024];
		int ret;
		while(ret != 0){
			ret = read(handler->getfd(),buf,sizeof(buf));
		}
		handler->disableAll();
		handler->removeSelf();
		close(handler->getfd());
		
		
	}
	
	/*void writeFunc(int fd){
		
	}
	
	void closeFunc(int fd){
		
		
	}
	
	void errFunc(int fd){
		
		
	}*/
	
	void TcpServer::createHandler(Reactor *reac,int fd){
		Handler *conHandler = new Handler(reac,fd);
		conHandler->setReadCallBack(std::bind(&TcpServer::readFunc,this,conHandler));  // 读完了之后就写。
		/*//conHandler.setWriteCallBack(...........);//暂时不用
		conHandler.setErrCallBack(...........);  //
		//conHandler.setCloseCallBack*/
		conHandler->enableRead();
		
	}
	
	
	void TcpServer::printAllHandler(){
		
		reactorPool->printAllHandler();
	}
	
	
	
	
	
	
	
	
	
}
