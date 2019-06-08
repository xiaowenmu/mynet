#include"include/ThreadPool.h"
#include<functional>
#include<vector>
#include<iostream>

namespace mynet{

	ThreadPool::ThreadPool(int nums):threadNums(nums),threadBlockingNum(0){
		RETCHECK(pthread_cond_init(&taskCondition,NULL));
	}
	
	ThreadPool::~ThreadPool(){}	

	void ThreadPool::run(){ //待改进，防止重复调用
		threads.reserve(threadNums);
		for(int i = 0; i < threadNums; ++i){
			threads.emplace_back(new mynet::Thread(std::bind(&mynet::ThreadPool::runInThread,this),i));
			threads[i]->run();
			
		}
	}
	
	
	ThreadPool::Task ThreadPool::getTask(){//获取任务，如果没有任务则线程阻塞，并ThreadBlockingNum + 1，代表阻塞的线程数
		Task ret;
		
		condMutex.lock();
		if(tasks.size() == 0){
			
			threadBlockingNum += 1;
			while(tasks.size() == 0){
				
				pthread_cond_wait(&taskCondition,condMutex.getMutex());
			}
			threadBlockingNum -= 1;
			ret = tasks.front();
			tasks.pop();
		}
		else{
			ret = tasks.front();
			tasks.pop();
		}
		condMutex.unlock();
		
		return ret;
			
		
	}
	
	
	
	void ThreadPool::push_task(Task task){//插入待处理的任务，如果线程饱和了就不发送信号，如果线程阻塞也就是没有任务要处理，那么插入一个任务就唤醒线程，这里没有用broadcast
		condMutex.lock();
		tasks.push(task);
		if(threadBlockingNum > 0)
			pthread_cond_signal(&taskCondition);//个人觉得放在外面用broadcast合适，因为放在外面的时候有可能还没发通知就又执行插入任务操作，防止有任务但是线程不知道的情况
		condMutex.unlock();
		
	}
	
	
	
	void ThreadPool::runInThread(){
		try{
			while(1){
				//std::cout<<" iam in"<<std::endl;
				//std::cout<<name<<std::endl;
				Task task(getTask());//取任务然后运行
				if (task){
					task();
				}
				//std::cout<<" iam in"<<std::endl;
			
			}
		}
		catch(...){
			
			fprintf(stderr, "unknown exception caught in ThreadPool %s\n", "thread");
			throw; // rethrow
		}
		
	}
	
	
	
	
}
