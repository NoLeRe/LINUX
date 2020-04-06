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
	int fd=open("./test.fifo",O_RDONLY);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	printf("open fifo succees\n");

	while(1)
	{

		char buf[1024]={0};
		int len=read(fd,buf,1023);
		if(len<0)
		{
			perror("read error");
			return -1;
		}else if(len==0)
		{
			perror("all write closed");
			return -1;
		}
		printf("read:[%s]\n",buf);
	}
	
	close(fd);
	return 0;	
}
