#include"include/AsynLog.h"
#include"include/LogFile.h"
#include"include/ScopeMutex.h"
#include<limits.h>
#include<unistd.h>



namespace mynet{
	
	
	
	AsynLog::AsynLog():thread(std::bind(&AsynLog::threadFunc,this)),mutex(),condition(mutex),curBuffer(new buffer),running(true){
		//curBufferList.reserve(16);
		
		freeBufferList.reserve(16);
		fullBufferList.reserve(16);
		freeBufferList.push_back(std::unique_ptr<buffer>(new buffer));
		freeBufferList.push_back(std::unique_ptr<buffer>(new buffer));
		freeBufferList.push_back(std::unique_ptr<buffer>(new buffer));
		
		thread.run();
	}
	
	AsynLog::~AsynLog(){
		running = false;
		condition.notify();
		thread.join();
		//while(1);
		//pthread_detach(thread.getThreadId());
	}
	
	
	void AsynLog::append(const char *str,size_t len){
		
		ScopeMutex lock(mutex);
		if (curBuffer->avail() > len){
			curBuffer->append(str, len);
		}
		else{
			fullBufferList.push_back(std::move(curBuffer));

			if (!freeBufferList.empty()){
				curBuffer = std::move(freeBufferList.back());
				freeBufferList.pop_back();
			}
			else{
				curBuffer.reset(new buffer); // Rarely happens
			}
			curBuffer->append(str, len);
			condition.notify();
		}
	}
	
	void AsynLog::threadFunc(){
		
		char baseName[NAME_MAX];
		char *ret = getcwd(baseName,sizeof(baseName));
		assert(ret != NULL); (void *)ret;
		LogFile file(baseName,rollSize,interval);
		std::vector<std::unique_ptr<buffer>> buffersToWrite;
		buffersToWrite.reserve(16);
		while(running){
			{
				ScopeMutex lock(mutex);
				if(fullBufferList.empty())//三秒刷新一次或者有数据来了
					condition.timedWait(interval);
				
				while(!buffersToWrite.empty()){//把空的buffer给freeBufferList
					freeBufferList.push_back(std::move(buffersToWrite.back()));
					buffersToWrite.pop_back();
				}
				
				fullBufferList.push_back(std::move(curBuffer));//把当前在用的buffer添加到fullBufferList，以便写入文件
				if(!freeBufferList.empty()){//给当前buffer一个新的buffer,先从freeBufferList中找，找不到就新建一个
					curBuffer = std::move(freeBufferList.back());
					freeBufferList.pop_back();
				}
				else{
					curBuffer.reset(new buffer);
				}
				buffersToWrite.swap(fullBufferList);
				
			}
			assert(!buffersToWrite.empty());
			
			if (buffersToWrite.size() > 25){
				char buf[256];
				snprintf(buf, sizeof buf, "Dropped log messages , %zd larger buffers\n",
						buffersToWrite.size()-2);
				fputs(buf, stderr);
				file.append(buf, static_cast<int>(strlen(buf)));
				buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
			}
			
			for (const auto& buffer : buffersToWrite){
      
				file.append(buffer->bufStart(), buffer->length());
				buffer->clear();
			}

			
			if (buffersToWrite.size() > 16){
				buffersToWrite.resize(16);
			}
			
			file.flush();
		}
		file.flush();
		
	}
	
	
	
}
