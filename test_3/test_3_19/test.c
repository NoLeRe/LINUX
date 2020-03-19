#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

	pid_t pid=fork();
	if(pid==0)
	{
		printf("%d \n",getpid());
		sleep(3);
		exit(42);
	}
	//wait(NULL);
	int ret,status;
	while(ret=waitpid(-1,&status,WNOHANG)==0)
	{	
		sleep(1);
		printf("----hurry up----\n");
	}
	printf("status:%d \n",status>>8&0xff);
	while(1)
	{
		printf("-----Don't Bother-----%d %d\n",getpid(),pid);
		sleep(1);
	}
	return 0;
}
