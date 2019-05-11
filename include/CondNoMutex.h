#ifndef CONDNOMUTEX_H
#define CONDNOMUTEX_H


#include<pthread.h>
#include"include/MacroDefine.h"
#include"include/Noncopyable.h"
#include"include/Mutex.h"


namespace mynet{
	
	
class CondNoMutex:public Noncopyable{
public:
	explicit CondNoMutex(Mutex &mu):mutex(mu){
		
		RETCHECK(pthread_cond_init(&cond,NULL);
		
	}
	~CondNoMutex(){
		
		RETCHECK(pthread_cond_destory(&cond);
	}
	
	void wait(){
		RETCHECK(pthread_cond_wait(&cond,mutex.getMutex());
		
	}
	
	
	void notify(){
		RETCHECK(pthread_cond_signal(&cond));
	}
	void notifyAll(){
		RETCHECK(pthread_cond_broadcast(&cond));
		
	}
	
	
	
private:
	Mutex &mutex;
	pthread_cond_t cond;
	
};
	
	
	
}










#endif