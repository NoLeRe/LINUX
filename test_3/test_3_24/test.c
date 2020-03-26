#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	pid_t pid=fork();
	if(pid==0)
	{
		printf("这是一个子进程\n");
		exit(42);
	}

	int status;
	wait(&status);
	printf("%d \n",status);

	return 0; 
}
