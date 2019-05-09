#include"include/Reactor.h"
#include"include/Handler.h"
#include<iostream>
#include<unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>   
 #include <fcntl.h>
int fd1;
int fd2;

mynet::Reactor a;
void *func(void *b){
	while(1){
	sleep(5);
	char buf = 'b';    //test fd
	write(fd1,&buf,sizeof(buf));
	sleep(2);
	write(fd2,&buf,sizeof(buf));
	a.wakeupReactor();// test wakeup function
}
	return (void *)0;
}


void fun(){
	char buf;
	read(fd1,&buf,sizeof(buf));
	std::cout<<"a"<<std::endl;
}
void fun1(){
	char buf;
	read(fd2,&buf,sizeof(buf));
	std::cout<<"c"<<std::endl;
}

int main(){

	
	mkfifo("aaaa",0777);
	mkfifo("cccc",0777);
	fd1 = open("aaaa",O_RDWR);
	fd2 = open("cccc",O_RDWR);
	pthread_t ff;
	pthread_create(&ff,NULL,func,NULL);
	
	mynet::Handler handler1(&a,fd1);
	mynet::Handler handler2(&a,fd2);
	handler1.setReadCallBack(fun);
	handler1.enableRead();
	handler2.setReadCallBack(fun1);
	handler2.enableRead();
	//handler1.disableAll();
	//handler1.removeSelf();
	//handler2.disableAll();
	//handler2.removeSelf();
	a.loop(-1);
	
	return 0;


}
