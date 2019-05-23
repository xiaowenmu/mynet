#ifndef REACTORTHREADPOOL_H
#define REACTORTHREADPOOL_H

#include"include/Noncopyable.h"
#include<memory>
#include<vector>

namespace mynet{
class Reactor;
class ReactorThread;
	
class ReactorThreadPool:public Noncopyable{
	
public:
	explicit ReactorThreadPool(int num);
	~ReactorThreadPool();
	
	void startThreadPool();
	Reactor *getNextReactor();
	Reactor *getMainReactor();

	void printAllHandler();
	void stopAll();
		
	

private:
	int threadNum;
	bool started;
	int nextReactor;
	std::vector<std::unique_ptr<ReactorThread> > reactorThreads;
	std::vector<Reactor *> allReactor;
	
	
	
	
};
	
	
	
	
	
	
	
}











#endif
