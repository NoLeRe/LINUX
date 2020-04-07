#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	int pipefd[2]={-1};
	int ret=pipe(pipefd);
	if(ret<0)
	{
		perror("pipe error");
		return -1;
	}

	pid_t pid=fork();
	if(pid==0)
	{
			close(pipefd[0]);	
			//sleep(1);
			char buf[1024]={0};
			int len=read(pipefd[0],buf,1023);
			printf("%s-%d\n",buf,len);
	}else{
		close(pipefd[0]);	
		char *ptr="hello world";
		int i=0;
		while(1)
		{	
			sleep(1);
			int len=write(pipefd[1],ptr,strlen(ptr));
			i+=len;
			printf("write success-%d\n",i);
		}
	}
	return 0;	
}

