#ifndef LOGFILE_H
#define LOGFILE_H

#include<string>
#include<stdlib.h>
#include<time.h>
#include"include/File.h"
#include<memory>


//负责更改文件，该类中包含了File类（该类负责写文件）

namespace mynet{

class LogFile{
	
public:
	LogFile(std::string name,size_t size,time_t inter);
	
	void append(const char *,size_t);
	
	bool rollFile();
	
	void flush();
	
	
	
	
private:
	std::string getFileName(time_t *);
	
	std::string baseName;//日志文件的绝对路径为baseName/...
	//string fileName;
	size_t rollSize;//日志文件多大时要换文件
	time_t interval;//刷新文件的间隔
	size_t lastRollDay;//上一次换文件的时间
	time_t lastFlush; //上一次刷新文件的时间
	time_t lastRollSecs;
	//static const time_t DaySeconds;
	std::unique_ptr<File> filePtr;
	static const time_t DaySeconds = 24 * 60 * 60;
	
	
	
	
	

	
	
};


}




#endif
