#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>


int main()
{
	umask(0);
	int ret=mkfifo("./test.fifo",0664);
	if(ret<0&&errno!=EEXIST)
	{
		perror("mkfifo error");
		return -1;
	}
	int fd=open("./test.fifo",O_WRONLY);
	
	while(1)
	{
		sleep(2);
		char *ptr="hello world";
		int len=write(fd,ptr,strlen(ptr));
		printf("write success\n");

	}
	close(fd);	
	return 0;	
}
