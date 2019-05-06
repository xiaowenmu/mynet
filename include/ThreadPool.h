#ifndef THREADPOOL_H
#define THREADPOOL_H

#include"include/MacroDefine.h"
#include"include/Mutex.h"
#include"include/Condition.h"
#include"include/Thread.h"
#include"include/Noncopyable.h"
#include<vector>
#include<memory>
#include<queue>

namespace mynet{
	
class ThreadPool:public Noncopyable{
public:
	typedef std::function<void()> Task;
	explicit ThreadPool(int nums);//创建多少个线程
	~ThreadPool();
	void push_task(Task);//插入待处理的任务
	void run();
	
	
private:
	void runInThread();//每个线程都运行这个函数，它的作用是取任务运行，如果没有任务那么它就睡眠
	Task getTask();
	int threadNums;
	int threadBlockingNum;
	std::vector<std::unique_ptr<mynet::Thread>> threads;//指向创建线程的指针
	std::queue<Task> tasks;//保存待执行的任务
	//Mutex taskMutex;//保护tasks插入和删除
	Mutex condMutex;
	//Condition taskCondition;//用于通知有tasks；
	pthread_cond_t taskCondition;//用于通知有tasks；
	
	
};
	
	
}





#endif
