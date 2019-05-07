#include"include/Handler.h"

namespace mynet{
	
	const int Handler::New     = -1; //代表没有加入过epoll的描述符
	//const int Handler::Modify  = 0;  //代表需要修改epoll的动作,修改关注的事情或者在epoll中删除该事件,Modify && noFocus == 在epoll中删除
	const int Handler::Deleted = 1;  //代表先加入了epoll后被删除的,但是在map中还存在（map保存在epoll中的，或者在epoll中被删除的）
	const int Handler::Added   = 2;  //代表已经在epoll中的
	
	const int Handler::noAttention = 0; //代表当前没有所关注的事情，可以在epoll中对其进行修改或删除
	const int Handler::readAttention = EPOLLIN | EPOLLPRI;//加不加EPOLLRDHUP都可以，关闭的时候可以通过读到EOF判断
	const int Handler::writeAttention = EPOLLOUT;
	
	const int Handler::nothingHappened = 0;
	
	
	Handler::Handler(const Reactor *reac,int fd1):reactor(reac),fd(fd1),action(New),happened(nothingHappened),focus(noAttention){
		
	}
	Handler::~Handler(){
		//close(fd);
	}
	
	
	
	void Handler::handleEvent(){
		//没有处理EPOLLHUP
		if(happened & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)){
			assert(readFunc);
			readFunc();
		}
		else if(happened & EPOLLOUT){
			assert(writeFunc);
			writeFunc();
		}
		else if(happened & EPOLLERR){
			assert(errFunc);
			errFunc();
			
		}
		
	}
}