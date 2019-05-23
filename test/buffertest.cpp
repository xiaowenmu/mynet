#include"include/Buffer.h"
#include<stdio.h>
#include<fcntl.h>
int main(){

	mynet::Buffer a;
	
	int fd = open("/home/tl/a.txt",O_RDWR);
	a.readFd(fd);
	int fd1 = open("/home/tl/a1.txt",O_RDWR);
	a.writeFd(fd1);
	return 0;


}
