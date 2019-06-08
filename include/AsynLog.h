#ifndef ASYNLOG_H
#define ASYNLOG_H

#include"include/Thread.h"
#include"include/Mutex.h"
#include"include/Condition.h"
#include"include/LogBuffer.h"
#include<functional>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<memory>

namespace mynet{


class AsynLog{

	
public:
	typedef LogBuffer<64*1024> buffer;
	typedef std::function<void ()> ThreadFunc;
	AsynLog();
	~AsynLog();

	void append(const char *,size_t len);


private:
	
	void threadFunc();
	Thread thread;
	Mutex mutex;
	Condition condition;
	//vector<std::unique_ptr<buffer>> curBufferList;//正在写的buffer
	std::vector<std::unique_ptr<buffer>> freeBufferList;//待写的buffer
	std::vector<std::unique_ptr<buffer>> fullBufferList;//前台写满的buffer防在这里
	std::unique_ptr<buffer> curBuffer;
	bool running;
	static const size_t rollSize = 12 *1024 * 1024;
	static const time_t interval = 3;
	
};





}




#endif
