#include"include/Buffer.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<algorithm>
#include<assert.h>
#include<sys/uio.h>
#include<errno.h>
namespace mynet{
	const size_t Buffer::BUFSIZE = 1024;//如果要修改则下面的getEndBias,getBias也要修改
	
	
	Buffer::Buffer():start(0),end(0),total(0){
		char *begin = (char *)malloc(BUFSIZE);
		assert(begin != NULL);
		total = BUFSIZE;
		bufferList.push_back(begin);
			
	}
	
	Buffer::~Buffer(){
		for(auto s : bufferList){
			free(s);
		}
		
	}
	
	char *Buffer::getStartPtr(){
		char *head = *bufferList.begin();
		return head + start;
	}
	
	
	
	
	/*char *Buffer::getEndPtr(){//如果刚好数据把其中一块空间填满了，就返回最后一个空间的最后一个地址。如果数据没填满就返回最后一个没填数据的地址
		size_t bias = getEndBias();
		if(bias == 0){
			size_t whichBlock = (end - 1) / BUFSIZE;
			char * head = *(bufferList.begin() + whichBlock);
			return head + BUFSIZE;
		}
		else{
			size_t whichBlock = end / BUFSIZE;
			char *head = *(bufferList.begin() + whichBlock);
			return head + bias;
		}
	}*/
	char *Buffer::getEndPtr(){//如果刚好数据把其中一块空间填满了，就返回下一块空间的首地址。如果数据没填满就返回最后一个没填数据的地址，指针总是在空间里面移动，不会移到外面去
			size_t bias = getEndBias();
			size_t whichBlock = end / BUFSIZE;
			std::list<char *>::iterator itr = bufferList.begin();
			advance(itr,whichBlock);
			char *head = *itr;
			return head + bias;
	}
	
	void Buffer::makeNewBlocks(size_t n){//重新申请n块空间，每一块大小为BUFSIZE大小,并且把它们挂到list上管理
		for(int i = 0; i < n; ++i){
			char *newBlock = (char *)malloc(BUFSIZE);
			bufferList.push_back(newBlock);
		}
		total += n * BUFSIZE;
		
	}
	
	void Buffer::ensureEnoughBytes(size_t n){ //确保还有n个字节的空间剩余，如果没有就分配空间
		size_t left = total - end;
		if(left < n){
			size_t needAllocBytes = n - left;
			size_t needBlocks = (needAllocBytes + BUFSIZE - 1) / BUFSIZE;
			makeNewBlocks(needBlocks);
			
		}
		
	}
	
	
	
	
	
	void Buffer::append(char *data,size_t n){ //往buffer里面写数据，从data开始写，写n个字节
		char *dataTemp = data;
		size_t bias = getEndBias();
		ensureEnoughBytes(n);
		size_t firstWriteBlockLeft = BUFSIZE - bias;
		size_t whichBlock = end / BUFSIZE;
		std::list<char *>::iterator itr = bufferList.begin();
		advance(itr,whichBlock);
		char *writeablePtr = *itr + bias;
		if(firstWriteBlockLeft >= n){
		
			memcpy((void *)writeablePtr,(void *)dataTemp,n);
			end += n;
		}
		else{
			memcpy((void *)writeablePtr,(void *)dataTemp,firstWriteBlockLeft);
			dataTemp += firstWriteBlockLeft;
			end += firstWriteBlockLeft;
			size_t leftWriteBytes = n - firstWriteBlockLeft;
			int circulTimes = leftWriteBytes / BUFSIZE;
			for(int i = 0; i < circulTimes; ++i){
				writeablePtr = getEndPtr();
				memcpy((void *)writeablePtr,(void *)dataTemp,(size_t)BUFSIZE);
				end += BUFSIZE;
				dataTemp += BUFSIZE;
				leftWriteBytes -= BUFSIZE;
				
			}
			
			if(leftWriteBytes != 0){
				writeablePtr = getEndPtr();
				memcpy((void *)writeablePtr,(void *)dataTemp,leftWriteBytes);
				end += leftWriteBytes;
			}
			
		}
	}
		
		
	size_t Buffer::discard(size_t n){//丢弃n个数据，如果总的数据量比n要小，丢弃全部的数据。函数返回实际丢弃的数据量
		size_t allDataBytes = end - start;
		if(n >= allDataBytes){
			start = 0;
			end = 0;
			return allDataBytes;
		}
		else{//n < allDataBytes
			size_t firstBlockDataBytes = std::min(BUFSIZE - start,allDataBytes);//有可能全部数据在第一块，有可能不是
			if(n < firstBlockDataBytes || end <= BUFSIZE){
				start += n;
			}
			else{
				size_t needDiscardBytes = n - firstBlockDataBytes;
				size_t needDiscardBlocks = 1 + needDiscardBytes/BUFSIZE;
				std::list<char *>::iterator itr = bufferList.begin();
				advance(itr,needDiscardBlocks);
				bufferList.splice(bufferList.begin(),bufferList,itr,bufferList.end());//注意是否有重叠区域
				size_t distance = allDataBytes - n;
				start = getBias(needDiscardBytes);
				end = start + distance;
				
			}
			return n;
		}
		
		
		
	}
	
	size_t Buffer::retrieve(char *buf,size_t n){//从buffer里面取数据，取n个字节,返回读到的字节数
		char *bufTemp = buf;
		size_t allDataBytes = end - start;
		size_t firstBlockDataBytes = std::min(BUFSIZE - start,allDataBytes);
		if(n >= allDataBytes){
			if(end <= BUFSIZE){//数据在一块内
				memcpy((void *)bufTemp,(void *)getStartPtr(),allDataBytes);
				
			}
			else{//数据分布了好几块
				memcpy((void *)bufTemp,(void *)getStartPtr(),firstBlockDataBytes);
				bufTemp += firstBlockDataBytes;
				size_t leftBytes = allDataBytes - firstBlockDataBytes;
				size_t fullBlockNums = leftBytes / BUFSIZE;
				std::list<char *>::iterator itr = bufferList.begin();
				for(int i = 0; i < fullBlockNums; ++i){
					++itr;
					memcpy((void *)bufTemp,(void *)*itr,BUFSIZE);
					bufTemp += BUFSIZE;
					leftBytes -= BUFSIZE;
					
				}
				++itr;
				
				memcpy((void *)bufTemp,(void *)*itr,leftBytes);
			}
			start = 0;
			end = 0;
			return allDataBytes;
			
			
		}
		else{//n < allDataBytes
			if( n < firstBlockDataBytes || end <= BUFSIZE){//数据在一块内
				memcpy((void *)bufTemp,(void *)getStartPtr(),n);
				start += n;
				
			}
			else{//数据分布了好几块
				memcpy((void *)bufTemp,(void *)getStartPtr(),firstBlockDataBytes);
				bufTemp += firstBlockDataBytes;
				size_t leftBytes = n - firstBlockDataBytes;
				size_t fullBlockNums = leftBytes / BUFSIZE;
				std::list<char *>::iterator itr = bufferList.begin();
				for(int i = 0; i < fullBlockNums; ++i){
					++itr;
					memcpy((void *)bufTemp,(void *)*itr,BUFSIZE);
					bufTemp += BUFSIZE;
					leftBytes -= BUFSIZE;
				}		
				++itr;
				memcpy((void *)bufTemp,(void *)*itr,leftBytes);
				size_t needDiscardBlocks = 1 + fullBlockNums;
				bufferList.splice(bufferList.begin(),bufferList,itr,bufferList.end());
				size_t distance = allDataBytes - n;
				start = leftBytes;
				end = start + distance;
				
				
				
			}
			return n;
			
			
			
		}
		
		
	}
	
	
	//返回Buffer中剩余的字节数
	size_t Buffer::readFd(int fd){
		char extraBuf[65536];
		size_t leftBytes = total - end;
		size_t freeBlocks = (leftBytes + BUFSIZE - 1)/BUFSIZE;
		int vecNums = freeBlocks+1;
		struct iovec vec[vecNums];
		if(isFull()){
			vec[0].iov_base = extraBuf;
			vec[0].iov_len = sizeof(extraBuf);
		}
		else{
			vec[0].iov_base = getEndPtr();
			vec[0].iov_len = BUFSIZE - getEndBias();
			size_t endBlock = end / BUFSIZE;
			std::list<char *>::iterator itr = bufferList.begin();
			advance(itr,endBlock);
			
			for(int i = 0; i < freeBlocks-1; ++i){
				++itr;
				vec[i+1].iov_base = *itr;
				vec[i+1].iov_len = BUFSIZE;
				
			}
			vec[freeBlocks].iov_base = extraBuf;
			vec[freeBlocks].iov_len = sizeof(extraBuf);
		}
		ssize_t n = readv(fd, vec, vecNums);
		if(n < 0){
			assert(false);
		}
		else if(n <= leftBytes){
				
			end += n;
		}
		else{//n > leftBytes
			end = total;
			append(extraBuf,n-leftBytes);
				
		}
		return n;
		//return end-start;
		
		
	}
	
	
	
	size_t Buffer::writeFd(int fd){
		size_t allBytes = end - start;
		if(allBytes == 0)
			return 0;
		size_t n;
		if(allBytes <= BUFSIZE - start){
			struct iovec vec;
			vec.iov_base = getStartPtr();
			vec.iov_len = allBytes;
			n = writev(fd,&vec,1);
			if(n == allBytes){
				start = 0;
				end = 0;
			}
			else if(n >= 0){
				start += n;
			}
			
		}
		else{//allBytes > BUFSIZE - start
			size_t firstBlockDataBytes = BUFSIZE - start;
			int vecNums = 1 + (allBytes - firstBlockDataBytes + BUFSIZE - 1) / BUFSIZE;
			struct iovec vec[vecNums];
			vec[0].iov_base = getStartPtr();
			vec[0].iov_len = BUFSIZE - start;
			std::list<char *>::iterator itr = bufferList.begin();
			for(int i = 0; i < vecNums-2; ++i){
				++itr;
				vec[i+1].iov_base = *itr;
				vec[i+1].iov_len = BUFSIZE;
				
			}
			++itr;
			vec[vecNums-1].iov_base = *itr;
			vec[vecNums-1].iov_len = getEndBias();
			n = writev(fd,vec,vecNums);
			if(n == allBytes){
				start = 0;
				end = 0;
			}
			else if(n >= 0 && n < BUFSIZE - start){
				 start += n;
			}
			else if(n >= BUFSIZE - start){
				size_t temp = n;
				temp -= (BUFSIZE - start);
				size_t blocks = 1;
				blocks += (temp / BUFSIZE);
				std::list<char *>::iterator itr = bufferList.begin();
				advance(itr,blocks);
				bufferList.splice(bufferList.begin(),bufferList,itr,bufferList.end());
				start = getBias(temp);
				end = (allBytes - n) + start;
			}
			
			
		}
		
		
			
			
		if(n < 0){
		
			switch(errno){
				//case EAGAIN:
				case EWOULDBLOCK:
				case EINTR:
					break;
				case EBADF:
				case EDESTADDRREQ:
				case EDQUOT:
				case EFAULT:
				case EFBIG:
				case EINVAL:
				case EIO:
				case ENOSPC:
				case EPIPE:
					assert(false);
					break;
				default:
					assert(false);
					break;
			
			}
		
		}
	}
	
}
