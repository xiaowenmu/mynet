#include"include/ThreadPool.h"
#include<iostream>
#include<cstdlib>
#include<functional>
#include"include/Timer.h"
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
//#include <stdio.h>
#include <stdint.h>
using namespace std;
void fun(){

	cout<<"1"<<endl;

}
void tun(){

	cout<<"2"<<endl;
}
int main(){

	mynet::ThreadPool thread(5);
	thread.run();
	//sleep(5);
	thread.push_task(tun);
	for(int i = 0; i < 50; ++i){
		thread.push_task(fun);
	}
	
	while(1);	

}
