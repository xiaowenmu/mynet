#ifndef REACTORTHREADPOOL_H
#define REACTORTHREADPOOL_H

#include"include/Noncopyable.h"
#include<memory>
#include<vector>

namespace mynet{
	
	
	
class ReactorThreadPool:public Noncopyable{
	
public:
	explicit ReactorThreadPool(int num);
	~ReactorThreadPool();
	
	void startThreadPool();
	Reactor *getNextReactor();


private:
	int threadNum;
	bool started;
	int nextReactor;
	std::vector<std::unique_ptr<ReactorThread> > reactorThreads;
	std::vector<Reactor *> allReactor;
	
	
	
	
};
	
	
	
	
	
	
	
}











#endif