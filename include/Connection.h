#ifndef CONNECTION_H
#define CONNECTION_H

#include"include/Noncopyable.h"
#include"include/Buffer.h"
#include<functional>
#include<memory>

namespace mynet{
	
class Reactor; 
class Handler;


class Connection:public Noncopyable,public std::enable_shared_from_this<Connection>{
public:
	typedef std::function<void (const std::shared_ptr<Connection> &con,Buffer *buffer)> MessageCallBack;
	static int Connected;
	static int Disconnecting;
	static int Disconnected;
	Connection(Reactor *,int);
	~Connection();
	
	
	
	
	void handleRead();

	void connectEstablished();

	void send(char *data,size_t len);

	void handleWrite();

	void setMessageCallBack(MessageCallBack &func){
		messageFunc = func;
	}
private:
	void shutdown();
	void handleClose();
	Reactor *reactor;
	int fd;
	Handler *handler;
	MessageCallBack messageFunc;
	Buffer outputBuffer;
	Buffer inputBuffer;
	int state;
	
	
	
	
};
	
typedef std::shared_ptr<Connection> ConnectionPtr;
	
}






#endif
