#include"include/Thread.h"
#include"include/MacroDefine.h"
namespace mynet{
	
	struct ThreadData{
		typedef std::function<void()> ThreadFun;
		ThreadFun fun;
		Condition *condPtr;
		pid_t *tid;
		ThreadData(ThreadFun f,Condition *cond,pid_t *t):fun(std::move(f)),condPtr(cond),tid(t){};
		
		void runInThread(){
			
			try{
				condPtr->notify();
				fun();
			}
			catch (...){
				fprintf(stderr, "unknown exception caught in Thread %s\n", "cuowo");
				throw; 
			}
		}
	};
	
	void *StartThread(void *dat){
		started = true;
		ThreadData *data = static_cast<ThreadData*>(dat);
		*(data->tid) = mynet::Thread::gettid();
		//condition->notify();
		//started = true;
		data->runInThread();
		delete data;
		return NULL;
		
	}
	
	Thread::Thread(ThreadFun f,int i):func(f),threadMutex(),condition(threadMutex),started(false),joined(false){
		char buf[20];
		snprintf(buf,sizeof(buf),"Thread %d",i);
		name = buf;
	};
	
	void Thread::run(){
		assert(started == false);
		//started = true;
		ThreadData *data = new ThreadData(func,&condition,&tid);
		if(pthread_create(&threadId,nullptr,StartThread,data)){
			delete data;
			started = false;
			ERRRET(-1);
		}
		else{
			pthread_detach(threadId);
			condition.wait();
		}
		return;
		
	}
	Thread::~Thread(){
		if (started && !joined){
			pthread_detach(threadId);
	}
}
	int Thread::join(){  
		if(started && !joined){
			joined = true;
			return pthread_join(threadId,NULL);
		}
		else 
			ERRRET(-1);
	}
	
	
	
}
