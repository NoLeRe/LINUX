#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signum)
{
	printf("recv a signal:%d \n",signum);	
}

int main()
{
	signal(SIGINT,sigcb);
	signal(SIGRTMIN+4,sigcb);

	sigset_t set;
	sigemptyset(&set);
	sigfillset(&set);

	sigprocmask(SIG_BLOCK,&set,NULL);
	printf("press entry continue\n");
	getchar();

	sigprocmask(SIG_UNBLOCK,&set,NULL);

	while(1)
	{
		sleep(1);	
	}
	return 0;
}
