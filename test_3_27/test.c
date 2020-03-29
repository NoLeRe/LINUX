#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fd=-1;
	umask(0);
	fd=open("test.txt",O_RDWR|O_CREAT,0777);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	lseek(fd,0,SEEK_END);
	char buf[1024]="kaman rider\n";
	int ret=write(fd,buf,strlen(buf));
	if(ret<0)
	{
		perror("write error");	
		return -1;
	}
	printf("ret = %d\n",ret);
	
	lseek(fd,0,SEEK_SET);
	char tmp[1024]={0};
	ret=read(fd,tmp,1023);
	if(ret<0)
	{
		perror("read error");
		return -1;
	}
	printf("ret =[%d],[\n%s]\n",ret,tmp);

	close(fd);
	return 0;
}
