#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include <sys/epoll.h>
#include<fcntl.h>
int main(){

	//for(int i = 0; i < 1000; ++i){
	int i = 0;
	int fdAll[5000];
	for(int i = 0; i < 5000; ++i){
		int fd = socket(AF_INET,SOCK_STREAM,0);
		if(fd < 0)
			printf("socket err\n");
		fdAll[i] = fd;
		struct sockaddr_in addr;
		memset(&addr,0,sizeof(addr));
	
		addr.sin_family = AF_INET;
	
		char b[] = "192.168.31.248";
		unsigned char buf[sizeof(struct in6_addr)];
		if(inet_pton(AF_INET,b,(void *)(&addr.sin_addr.s_addr))<0){
			printf("err 1\n");
		}
		addr.sin_port = htons(60000);

		if(connect(fd,(struct sockaddr*)(&addr),sizeof(sockaddr_in)) < 0 ){
			printf("err 2\n");
			printf("%s\n",strerror(errno));	
		}
		printf("%d\n",i);
		
	}

	//while(1);
	int epollFd = epoll_create(1);
	if(epollFd < 0)
		printf("epoll err\n");
	
	for(int i = 0; i < 5000; ++i){
		struct epoll_event eve;
		memset(&eve,0,sizeof(eve));
		eve.events = EPOLLIN;
		eve.data.fd = fdAll[i];
		if(epoll_ctl(epollFd,EPOLL_CTL_ADD,fdAll[i],&eve) < 0)
			printf("ctl err\n");

	}


	for(int i = 0; i < 5000; ++i){
		char buf6[5];
		buf6[0] = '1';
		buf6[1] = '2';
		write(fdAll[i],buf6,1);
		//printf("write\n");
	}

	struct epoll_event happen[5000];
	while(1){
	int nums = epoll_wait(epollFd,happen,5000,-1);
	if(nums > 0){
		for(int i = 0; i < nums; ++i){
			char fdBuf[50];
			snprintf(fdBuf, sizeof(fdBuf), "/home/tl/readfile/%d", happen[i].data.fd);
			int writeFd = open(fdBuf,O_RDWR|O_APPEND|O_CREAT,0777);
			if(writeFd < 0)
				printf("open err\n");
			char readbuf[65536];
			int ret = read(happen[i].data.fd,readbuf,sizeof(readbuf));
			write(writeFd,readbuf,ret);
			close(writeFd);
		}

	}
	else if(nums < 0){
		printf("wait err\n");
	}

	}
	

	
	
	//printf("%s\n",buf6);


	//}
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
