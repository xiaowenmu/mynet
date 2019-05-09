#include"include/Handler.h"
#include"include/Reactor.h"
#include<assert.h>
#include<sys/epoll.h>
#include<unistd.h>




namespace mynet{
	
	const int Handler::New     = -1; //代表没有加入过epoll的描述符
	//const int Handler::Modify  = 0;  //代表需要修改epoll的动作,修改关注的事情或者在epoll中删除该事件,Modify && noFocus == 在epoll中删除
	const int Handler::Deleted = 1;  //代表先加入了epoll后被删除的,但是在map中还存在（map保存在epoll中的，或者在epoll中被删除的）
	const int Handler::Added   = 2;  //代表已经在epoll中的
	
	const int Handler::noAttention = 0; //代表当前没有所关注的事情，可以在epoll中对其进行修改或删除
	const int Handler::readAttention = EPOLLIN | EPOLLPRI;//加不加EPOLLRDHUP都可以，关闭的时候可以通过读到EOF判断
	const int Handler::writeAttention = EPOLLOUT;
	
	const int Handler::nothingHappened = 0;
	

	
	Handler::Handler(Reactor *reac,int fd1):reactor(reac),fd(fd1),action(New),happened(nothingHappened),focus(noAttention){
		
	}
	Handler::~Handler(){
		assert(action != New);
		disableAll();
		removeSelf();
		close(fd);
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
	
	void Handler::removeSelf(){
		reactor->removeHandler(this);
	}
	
	void Handler::enableRead(){
		focus |= readAttention;
		reactor->updateHandler(this);
		
	}
	void Handler::enableWrite(){
		focus |= writeAttention;
		reactor->updateHandler(this);
	}
	void Handler::disableRead(){
		focus &= ~readAttention;
		reactor->updateHandler(this);
	}
	void Handler::disableWrite(){
		focus &= ~writeAttention;
		reactor->updateHandler(this);
	}
	void Handler::disableAll(){//如果action==New or ==Deleted ,就不做任何动作（因为把这个添加进epoll里面也没什么用）,如果是Added就把这个文件描述符从epoll中删除
		focus = noAttention;
		if(action == Added){
			
			reactor->updateHandler(this);
		}
	}
}
