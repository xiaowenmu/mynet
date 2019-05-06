#ifndef CONDITION_H
#define CONDITION_H

#include<pthread.h>
#include"include/MacroDefine.h"
#include"include/Noncopyable.h"
#include"include/Mutex.h"

namespace mynet{
	
class Condition:public Noncopyable{
public:
	explicit Condition(Mutex &mut):mutex(mut),count(0){
		RETCHECK(pthread_cond_init(&cond,NULL));	
	}
	~Condition(){
		RETCHECK(pthread_cond_destroy(&cond));
	}
	void wait(){
		mutex.lock();
		while(count != 1){
			RETCHECK(pthread_cond_wait(&cond,mutex.getMutex()));
		}
		count = 0;
		mutex.unlock();
		
	}
	void notifyAll(){
		mutex.lock();
		count = 1;
		mutex.unlock();
		RETCHECK(pthread_cond_broadcast(&cond));
	}
	void notify(){
		mutex.lock();
		count = 1;
		mutex.unlock();
		RETCHECK(pthread_cond_signal(&cond));
	}
	bool timedWait(int sec);


private:
	Mutex &mutex;
	pthread_cond_t cond;
	int count;

};
	
	
}






#endif
