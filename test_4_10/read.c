#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
	umask(0);
	int ret=mkfifo("./test.fifo",O_CREAT|0664);
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
	while(1)
	{
		sleep(15);
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

	return 0;
}
