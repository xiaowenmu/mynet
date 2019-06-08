#ifndef LOGBUFFER_H
#define LOGBUFFER_H


#include<stdlib.h>
#include<string.h>

namespace mynet{
	
	
	
template<int SIZE>	
class LogBuffer{
	
	
public:
	explicit LogBuffer():nums(0){}
	
	void append(const char *buf,size_t len){
		
		memcpy(buffer+nums,buf,len);
		nums += len;
		
	}
	
	char *bufStart(){
		return buffer;
	}
	
	size_t length(){
		return nums;
		
	}
	
	void clear(){
		nums = 0;
		
	}
	
	size_t avail(){
		
		return SIZE - nums;
	}

private:
	static const size_t BufferSize;
	char buffer[SIZE];
	size_t nums;
	
	
	
	
	
	
};
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}









#endif
