#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int pipefd[2]={-1};
	int ret=pipe(pipefd);
	if(ret<0)
	{
		perror("pipe error");
		return -1;
	}
	pid_t pid_ps=fork();
	if(pid_ps==0)
	{
		dup2(pipefd[1],1);
		execlp("ps","ps","-ef",NULL);
		exit(0);
	}
	pid_t pid_g=fork();
	if(pid_g==0)
	{
		close(pipefd[1]);
		dup2(pipefd[0],0);
		execlp("grep","grep","ssh",NULL);
		exit(0);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_ps,NULL,0);
	waitpid(pid_g,NULL,0);

	return 0;	
}
