//#include"include/Mutex.h"
//#include"include/Condition.h"
//#include"include/ScopeMutex.h"
#include"include/AsynLog.h"
//#include"include/File.h"
//#include"include/LogBuffer.h"
//#include"include/LogFile.h"
#include"include/LogStream.h"
#include<string>


mynet::AsynLog log;

void outputFunc(const char* msg, int len)
{
  log.append(msg, len);
}

int main(){

	//mynet::AsynLog log;
	mynet::LogStream::setOutput(outputFunc);


	 //line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

  	for (int i = 0; i < 1000000; ++i){
   		LOG_INFO << "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ " ;

  	  	usleep(100);
  	}
	return 0;

}
