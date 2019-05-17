#ifndef BUFFER_H
#define BUFFER_H

#include<list>
#include"include/Noncopyable.h"
#include<stdio.h>


//buffer的设计思想：
//用链表来管理一块块BUFSIZE大小的空间，链表中保存空间的首地址，第一个有数据的指针一定在链表的头结点中。（设计暂时不考虑回收多余的空间）
//非线程安全
//
//
//
//
//
//
//





namespace mynet{



	
class Buffer:public Noncopyable{
	
public:
	Buffer();
	~Buffer();
	

		
	void append(char *data,size_t n);
		
	size_t discard(size_t n);
		
	size_t retrieve(char *buf,size_t n);
		
	size_t readFd(int fd);
	

private:
	
	
	size_t getEndBias(){//求余操作,如果BUFSIZE变了，这个也要变。求end在块中的偏移
		
		return end & 0x3FF;
	}
	
	size_t getBias(size_t num){
		
		return num & 0x3FF;
	}
	
	bool isFull(){
		return end == total;
	}
	
	char *getStartPtr();
	
	char *getEndPtr();	
	
	void makeNewBlocks(size_t n);
		
	void ensureEnoughBytes(size_t n);
	
	std::list<char *> bufferList;
	static const size_t BUFSIZE;
	size_t start;//用int而不是char *来指定起始位和开始位的目的是地址可能是不连续的，不能通过start递增操作来访问元素
	size_t end;  //这里用unsigned int 就足够了，它一共可以代表4095G的空间。
	size_t total;//总空间大小
	
	
};
	
	
	
	
}


#endif
