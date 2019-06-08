#ifndef FILE_H
#define FILE_H

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string>

namespace mynet{


//只负责写文件
class File{
	
public:

	explicit File(std::string name);
	~File();
	
	void append(const char *,size_t);
	
	
	size_t allWritenBytes(){
		return writenBytes;
	}
	
	void flush();




private:
	FILE *fp;
	std::string fileName;
	char buffer[64*1024];
	size_t writenBytes;
	//static const writenLimit;
	
};







}







#endif
