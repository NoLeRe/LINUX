#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signum)
{
	pid_t pid;
	while(pid=(waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("destroy child process:%d\n",pid);	
	}
	printf("signum:%d\n",signum);
}

int main()
{
	signal(SIGCHLD,sigcb);

	pid_t pid=fork();
	if(pid==0)
	{
		sleep(5);
		exit(0);
	}
	pid=fork();
	if(pid==0)
	{
		sleep(9);
		exit(0);
	}
	while(1)
	{
		sleep(1);
		printf("别烦我-----\n");	
	}
	return 0;	
}
