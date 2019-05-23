#include"include/Connection.h"
#include<errno.h>
#include"include/Handler.h"
#include<assert.h>
#include<unistd.h>
namespace mynet{
	
	int Connection::Connected = 1;
	int Connection::Disconnecting = 2;
	int Connection::Disconnected = 3;
	
	Connection::Connection(Reactor *rec,int fd1):reactor(rec),fd(fd1),handler(new Handler(rec,fd1)),state(Connected){
		handler->setReadCallBack(std::bind(&Connection::handleRead,this));
		handler->setWriteCallBack(std::bind(&Connection::handleWrite,this));
		//handler.setErrCallBack();
		//handler.setCloseCallBack();
		
	}
	Connection::~Connection(){
		close(fd);
	}
	
	
	void Connection::connectEstablished(){
		
		assert(state == Connected);
		handler->tie(shared_from_this());
		handler->enableRead();
	}

	
	
	
	//在messageFunc中要处理完所有完整的请求
	void Connection::handleRead(){
		size_t n = inputBuffer.readFd(handler->getfd());
		if (n > 0){
			assert(messageFunc);
			messageFunc(shared_from_this(),&inputBuffer);
		}
		else if (n == 0){//inputBuffer为空
			handleClose();
		}
		
	}
	
	
	void Connection::send(char *data,size_t len){
		
		size_t ret = write(handler->getfd(),data,len);
		if(ret >= 0 && ret < len){
			outputBuffer.append(data+ret,len-ret);
			handler->enableWrite();
		}
		else if(ret < 0){
			
			switch(errno){
				case EAGAIN:
				//case EWOULDBLOCK:
				case EINTR:
					outputBuffer.append(data,len);
					handler->enableWrite();
					break;
				case EBADF:
				case EDESTADDRREQ:
				case EDQUOT:
				case EFAULT:
				case EFBIG:
				case EINVAL:
				case EIO:
				case ENOSPC:
				case EPIPE:
					assert(false);
					break;
				default:
					assert(false);
					break;
			
			}
			
		}
		
		
	}
	
	
	
	//注意：handleRead里的messageFunc可能只能一次处理一条信息，所以要想办法把信息处理完
	
	void Connection::handleWrite(){
		outputBuffer.writeFd(handler->getfd());
		
		if (outputBuffer.isEmpty()){
			handler->disableWrite();
		
			if (state == Disconnecting){
				shutdown();
			}
		}
      
    }
	
	void Connection::shutdown(){
		state = Disconnected;
		handler->disableAll();
		handler->removeSelf();
		ConnectionPtr guardThis(shared_from_this());
		delete(handler);
		
	}
	
	
	void Connection::handleClose(){
		assert(inputBuffer.isEmpty());
		state = Disconnecting;
		if(outputBuffer.isEmpty()){
			shutdown();
		}
	}


	
}
