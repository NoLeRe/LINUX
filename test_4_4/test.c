#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int pipefd[2]={-1};
	int fd=pipe(pipefd);
	if(fd<0)
	{
		perror("pipe error");
		return -1;
	}
	pid_t pid=fork();
	if(pid==0)
	{

		close(pipefd[1]);
		
		sleep(3);
		
		char str[1024]={0};
		int len=read(pipefd[0],str,1023);
		printf("buf-[%d][%s]\n",len,str);
	}
	else
	{
		
		close(pipefd[1]);
		
		sleep(5);
		char *buf="hello world";
		int total=0;
		while(1)
		{	
			int len=write(pipefd[1],buf,strlen(buf));
			total+=len;
			sleep(1);
			printf("total len=%d \n",total);
		}	
	}
	
	return 0;	
}
