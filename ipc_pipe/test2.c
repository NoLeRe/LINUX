#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<errno.h>

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
	if(fd<0)
	{
		perror("open error");
		return -1;
	}

	char *ptr="hello world";
	while(1)
	{
		int len=write(fd,ptr,strlen(ptr));
		printf("write data success\n");
		sleep(1);
	}
	close(fd);
	return 0;
}
