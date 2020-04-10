#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>

int main()
{
	umask(0);
	int ret=mkfifo("test.fifo",O_CREAT|0664);
	if(ret<0&&errno!=EEXIST)
	{
		perror("mkfifo error");
		return -1;
	}
	int fd=open("test.fifo",O_WRONLY);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	char *ptr="hello world";
	while(1)
	{
		
		sleep(1);
		
		int len=write(fd,ptr,strlen(ptr));
		if(len<0)
		{
			error("write error");
			return -1;
		}
		printf("write success\n");
		
	}

	return 0;
}
