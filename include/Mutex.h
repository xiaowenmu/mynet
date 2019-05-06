#ifndef MUTEX_H
#define MUTEX_H

#include<pthread.h>
#include<assert.h>
#include"include/MacroDefine.h"
#include"include/Noncopyable.h"




namespace mynet{

class Mutex : public Noncopyable
{
public:
	Mutex(){
		RETCHECK(pthread_mutex_init(&mutex,NULL));
	}
	~Mutex(){
		RETCHECK(pthread_mutex_destroy(&mutex));
	}
	void lock(){
		RETCHECK(pthread_mutex_lock(&mutex));
	}
	void unlock(){
		RETCHECK(pthread_mutex_unlock(&mutex));
	}
	pthread_mutex_t *getMutex(){
		return &mutex;
	}


private:
	pthread_mutex_t mutex;
	

};





}



#endif
