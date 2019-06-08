#ifndef LOGSTREAM_H
#define LOGSTREAM_H


#include"include/LogBuffer.h"
#include<limits.h>
#include<functional>
#include<stdlib.h>
namespace mynet{
	
	
	
class LogStream{
	
public:

	
	typedef std::function<void (const char *,size_t)> OutputFunc;
	
	LogStream(const char *,int,const char *);
	LogStream(const char *,int,const char *,bool);
	
	~LogStream();
	
	LogStream& operator<<(const char *); 
	//void setOutput(OutputFunc out); //设置输出函数
	static void setOutput(OutputFunc fun);

	

private:
	void formatTime();
	char file[NAME_MAX+24];//保存文件名和行号
	size_t nameLen;//保存文件名和行号的长度
	char info[12];//保存级别
	size_t infoLen;//保存级别长度
	size_t totalLen;//保存一条日志的长度
	bool canAbort;
	LogBuffer<1024> buffer;
	static OutputFunc output;
	static const int kMicroSecondPerSecond = 1000 * 1000;
	
};



#define LOG_TRACE mynet::LogStream(__FILE__, __LINE__,    "TRACE    ")
#define LOG_DEBUG mynet::LogStream(__FILE__, __LINE__,    "DEBUG    ")
#define LOG_INFO mynet::LogStream(__FILE__, __LINE__,     "INFO     ")
#define LOG_WARN mynet::LogStream(__FILE__, __LINE__,     "WARN     ")
#define LOG_ERROR mynet::LogStream(__FILE__, __LINE__,    "ERROR    ")
#define LOG_FATAL mynet::LogStream(__FILE__, __LINE__,    "FATAL    ")
#define LOG_SYSERR mynet::LogStream(__FILE__, __LINE__,   "SYSERR   ",false)
#define LOG_SYSFATAL mynet::LogStream(__FILE__, __LINE__, "SYSFATAL ",true)




	
	
	
	
	
	
	
	
	
}










#endif
