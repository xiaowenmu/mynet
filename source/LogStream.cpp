#include"include/LogStream.h"
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<sys/time.h>
namespace mynet{
	__thread char t_time[64];
	__thread time_t t_lastSecond = 0;
	
	
	void defaultOutput(const char *str,size_t len){
		fprintf(stderr,"%s",str);
	}
	
	mynet::LogStream::OutputFunc LogStream::output = defaultOutput;
	
	
	
	LogStream& LogStream::operator<<(const char *str){
		size_t len = strlen(str);
		buffer.append(str,len);
		totalLen += len;
		
		return *this;
	}
	
	
	
	LogStream::LogStream(const char *fileName,int lineNum,const char *inf):nameLen(0),infoLen(0),totalLen(0),canAbort(false){
		char line[24];
	   	size_t lineLen = snprintf(line,sizeof(line),":%d\n",lineNum);
		nameLen = strlen(fileName);
		file[0] = ' ';
		memcpy(file+1,fileName,nameLen);
		memcpy(file+nameLen+1,line,lineLen);
		nameLen += lineLen + 1;
		infoLen = strlen(inf);
		memcpy(info,inf,infoLen);
		formatTime();
		buffer.append(info,infoLen);
		totalLen += infoLen; 
		
		
		
		
	}
	
	
	LogStream::~LogStream(){
		buffer.append(file,nameLen);
		
		totalLen += nameLen;
		
		output(buffer.bufStart(),totalLen);
		if(canAbort)
			abort();
	}
	
	
	
	LogStream::LogStream(const char *fileName,int lineNum,const char *inf,bool abo):nameLen(0),infoLen(0),totalLen(0),canAbort(abo){
		char line[24];
	    	size_t lineLen = snprintf(line,sizeof(line),":%d\n",lineNum);
		nameLen = strlen(fileName);
		file[0] = ' ';
		memcpy(file+1,fileName,nameLen);
		memcpy(file+nameLen+1,line,lineLen);
		nameLen += lineLen + 1;
		infoLen = strlen(inf);
		memcpy(info,inf,infoLen);
		formatTime();
		buffer.append(info,infoLen);
		totalLen += infoLen; 
		
	}
	

	
	void LogStream::formatTime(){
		struct timeval tv;
		gettimeofday(&tv, NULL);
		int64_t microSecondsSinceEpoch = tv.tv_sec * kMicroSecondPerSecond + tv.tv_usec;
		time_t seconds = tv.tv_sec;
		int microseconds = static_cast<int>(tv.tv_usec);
		if (seconds != t_lastSecond){
			t_lastSecond = seconds;  //t_lastSecond为上一次转化的秒数，如果这一次的和上一次的相同，那么就不需要再对年月日时分秒进行格式化了，只格式化ms为字符串形式
			struct tm tm_time;
			
			
			gmtime_r(&seconds, &tm_time); // 把从epoch经过的秒数转换成tm结构体的形式   gmtime_r为线程安全，gmtime不是线程安全的
			

			int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",    //17bytes
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);           //把tm结构体转换为字符串形式
			assert(len == 17); (void)len;
		}

		
		int n = snprintf(t_time+17,sizeof(t_time)-17,".%06dZ ", microseconds);
		assert(n == 9);
		buffer.append(t_time,17+9);
		totalLen += 17 + 9;
		
	}
	
	
	void LogStream::setOutput(OutputFunc fun){
		output = fun;
	}
	
}
