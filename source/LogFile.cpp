#include"include/LogFile.h"
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<time.h>
#include<limits.h>

namespace mynet{
	
	
	

	LogFile::LogFile(std::string name,size_t size,time_t inter):baseName(name),rollSize(size),interval(inter),lastRollDay(0),lastFlush(0),lastRollSecs(0){
		struct stat sta;
		stat(baseName.c_str(),&sta);
		assert(S_ISDIR(sta.st_mode));
		rollFile();
		
	}
	
	
	void LogFile::append(const char *buf,size_t len){
		filePtr->append(buf,len);
		if(filePtr->allWritenBytes() >= rollSize){
			rollFile();
		}
		else{//检查是否超时
			
			time_t now = time(NULL);
			size_t days = now / DaySeconds;
			if(days != lastRollDay){
				rollFile();
			}
			/*else if(now-interval >= lastFlush){
				filePtr->flush();
				lastFlush = now;
			}*/
			
		}
		//filePtr->append(buf,len);
	}
	
	
	bool LogFile::rollFile(){
		time_t now;
		std::string fileName = getFileName(&now);
		size_t days = now / DaySeconds;
		//if(now < lastRollSecs)//这里是要防止用户更改系统时间
		//	return false;
		lastRollDay = days;
		lastRollSecs = now;
		filePtr.reset(new File(fileName));
		return true;
	}
	
	std::string LogFile::getFileName(time_t *now){
		//string name;
		//name.reserve(baseName.size() + 64);
		char save[NAME_MAX];
		struct tm transf;
		*now = time(NULL);
		localtime_r(now,&transf);
		strftime(save, sizeof(save), "/%Y%m%d-%H%M%S.log", &transf);
		return baseName + save;
	}
	
	
	void LogFile::flush(){

		filePtr->flush();
	}
	
	
	
	
	
	
	
	
	
}
