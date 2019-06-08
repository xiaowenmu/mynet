#include"include/File.h"
#include<stdio.h>
#include<assert.h>
#include<string.h>
namespace mynet{
	
	
	
	//const File::writenLimit = 60 * 1024;
	
	File::File(std::string name):fileName(name),writenBytes(0){
		fp = fopen(fileName.c_str(), "a");//创建并打开文件
		assert(fp != NULL);
		setbuffer(fp, buffer, sizeof(buffer));//设置缓冲区
	}
	File::~File(){
		fclose(fp);
	}
	
	void File::append(const char *bytes,size_t len){
		size_t n = fwrite_unlocked(bytes, 1, len, fp);
		size_t left = len - n;
		while(left > 0){
			n = fwrite_unlocked(bytes, 1, left, fp);
			if(n == 0){
				int err = ferror(fp);
				if(err){
					char errInfo[90];
					strerror_r(err,errInfo,sizeof(errInfo));
					fprintf(stderr, "%s",errInfo);
					break;
				}
				
			}
			left -= n;
		}
		writenBytes += len;
		
	}
	
	void File::flush(){
		fflush(fp);
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
