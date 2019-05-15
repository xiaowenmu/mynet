#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>

int main(){

	for(int i = 0; i < 1000; ++i){

	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(fd < 0)
		printf("socket err\n");
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	//addr.sa_len = 16;
	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = htonl(INADDR_ANY);
	char b[] = "192.168.140.133";
	unsigned char buf[sizeof(struct in6_addr)];
	if(inet_pton(AF_INET,b,(void *)(&addr.sin_addr.s_addr))<0){
		printf("err 1\n");
	}
	addr.sin_port = htons(60000);

	if(connect(fd,(struct sockaddr*)(&addr),sizeof(sockaddr_in)) < 0 ){
		printf("err 2\n");
		printf("%s\n",strerror(errno));	
	}
	


	}
	/*int ret = bind(fd,(struct sockaddr*)(&addr),sizeof(sockaddr_in));
	if(ret == -1){
		printf("bind err\n");
		printf("%s\n",strerror(errno));	
	}
	ret = listen(fd,SOMAXCONN);
	if(ret < 0)
		printf("listen err\n");
	*/
	while(1);


}
