#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void sigcb(int signum)
{
	printf("recv a signal:%d\n",signum);	
}

int main()
{
	signal(SIGINT,sigcb);
	signal(SIGRTMIN+4,sigcb);
	sigset_t set;
	//sigprocmask(SIG_BLOCK,&set,NULL);

	sigemptyset(&set);
	sigfillset(&set);
	
	sigprocmask(SIG_BLOCK,&set,NULL);
	getchar();
	sigprocmask(SIG_UNBLOCK,&set,NULL);
	while(1)
	{
	}

	return 0;	
}
