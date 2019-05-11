#ifndef SCOPEMUTEX_H
#define SCOPEMUTEX_H


#include"include/Noncopyable.h"
#include"include/Mutex.h"

namespace mynet{
	
	
class ScopeMutex:public Noncopyable{

public:
	explicit ScopeMutex(Mutex &sour):mutex(sour){
		mutex.lock();
	}
	~ScopeMutex(){
		mutex.unlock();
	}

private:
	Mutex &mutex;
};
	

	
	
	
}


#define ScopeMutex(x) ("error can not create temp ScopeMutex") //如果创建了一个临时ScopeMutex对象，就会导致编译时出错



#endif