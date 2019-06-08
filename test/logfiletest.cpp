#include"include/LogFile.h"
#include<unistd.h>


int main(){


	mynet::LogFile file("/home/tl/1gittest",1024,3);
	char buf[] = "adfakdhfakjdfhakdjhfaiuerhaiehadaiduhfaidhfi11111111111111111111111111111111sdfjasdjf111\n";
	for(int i = 0; i < 1000; ++i){
		file.append(buf,sizeof(buf));
		usleep(1000);
	}
	return 0;

};
