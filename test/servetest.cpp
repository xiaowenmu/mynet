#include<sys/types.h>
#include<sys/socket.h>
#include"include/TcpServer.h"
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include"include/Connection.h"
#include<functional>
#include"include/Buffer.h"
#include<fcntl.h>
//typedef std::shared_ptr<Connection> ConnectionPtr;
void messageFunc(const mynet::ConnectionPtr &con,mynet::Buffer *buffer){
	char buf[20];
	buffer->retrieve(buf,20);
	printf("receive %s \n",buf);
	buf[0] = '1';
	buf[1] = '2';
	buf[3] = '3';
	buf[4] = '4';
	buf[5] = '\0';
	char buf1[2000];
	int fd = open("/home/tl/a.txt",O_RDWR);
	size_t ret = 1;
	while(ret != 0){
		ret = read(fd,buf1,sizeof(buf1));
		con->send(buf1,ret);
		printf("ret is %u\n",ret);
	}
	return ;

}
int main(){

	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(60000);
	mynet::TcpServer server(6,(struct sockaddr *)(&addr));
	server.setMessageCallBack(std::bind(&messageFunc,std::placeholders::_1,std::placeholders::_2));
	
	server.start();

	
		


	
	while(1);

}
