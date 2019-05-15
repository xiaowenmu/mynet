#include"include/Accept.h"
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>

#include<iostream>

namespace mynet{
	int createSocket(sa_family_t family){
		int fd = socket(family,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
		assert(fd > 0);
		return fd;
		
	}
	
	
	
	void setReuseAddr(const int &fd){
		int optval = 1;
		int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,&optval, static_cast<socklen_t>(sizeof optval));
		assert(ret != -1);
    }

	
	void setReusePort(const int &fd){
#ifdef SO_REUSEPORT
		int optval = 1;
		int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,&optval, static_cast<socklen_t>(sizeof optval));
		assert(ret != -1);
#else
		assert(false);
#endif
	}

	void setKeepAlive(const int &fd){
		int optval = 1;
		setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,&optval, static_cast<socklen_t>(sizeof optval));
 
	}

	void bindAddr(const int &fd,const struct sockaddr *addr){
		if(addr->sa_family == AF_INET){
			int ret = bind(fd,addr,sizeof(struct sockaddr_in));
			assert(ret != -1);
		}
		else if(addr->sa_family == AF_INET6){
			int ret = bind(fd,addr,sizeof(struct sockaddr_in6));
			assert(ret != -1);
		}
	}
	
	
	Accept::Accept(Reactor *reac,struct sockaddr *addr):reactor(reac),acceptFd(createSocket(addr->sa_family)),acceptHandler(reac,acceptFd),listenning(false),times(0){
		assert(reactor != nullptr);
		setReuseAddr(acceptFd);
		setReusePort(acceptFd);
		setKeepAlive(acceptFd);
		bindAddr(acceptFd,addr);
		acceptHandler.setReadCallBack(std::bind(&Accept::handleRead,this));
		
		
	}
	Accept::~Accept(){
		acceptHandler.disableAll();
		acceptHandler.removeSelf();
		close(acceptFd);
		
	}
	
	void Accept::listen(){
		assert(reactor != nullptr);
		assert(listenning == false);
		
		listenning = true;
		int ret = ::listen(acceptFd, SOMAXCONN);
		if (ret < 0){
			assert(-1);
		}
		acceptHandler.enableRead();//放在reactor中执行
		
	}
	
	Accept* Accept::getPointer(){
		return this;
	}
	
	void Accept::handleRead(){
		struct sockaddr_in6 peerAddr;
		socklen_t len = sizeof(peerAddr);
		int fd = accept(acceptFd,(struct sockaddr*)(&peerAddr),&len);
		if(fd >= 0){
			
			if (newCallBack){
				std::cout<<++times<<std::endl;
				newCallBack(fd);
			}
			else{
				close(fd);
			}
			
		}
		else{
			switch (errno){
				case EAGAIN:
				case ECONNABORTED:
				case EINTR:
				case EPROTO: 
				case EPERM:
				case EMFILE: 
					break;
				case EBADF:
				case EFAULT:
				case EINVAL:
				case ENFILE:
				case ENOBUFS:
				case ENOMEM:
				case ENOTSOCK:
				case EOPNOTSUPP:
					assert(-1);
					break;
				default:
					assert(-1);
					break;
			}
			
		}
		
		
	}
	
}
