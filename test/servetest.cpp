#include<sys/types.h>
#include<sys/socket.h>
#include"include/TcpServer.h"
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
int main(){

	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(60000);
	mynet::TcpServer server(6,(struct sockaddr *)(&addr));
	server.start();

	sleep(2);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	printf("\n");
	sleep(10);
	server.printAllHandler();
	sleep(10);
	server.printAllHandler();
	printf("\n");
	printf("\n");
	while(1);
	return 0;


}
