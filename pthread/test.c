#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


void function()
{
	char *str="this pthread exit\n";
	pthread_exit((void*)str);
}

void *pthread_1(void* arg)
{
	pthread_detach(pthread_self());
	while(1)
	{
		printf("this is pthread----%s",arg);
		sleep(1);
		function();
	}
	return NULL;
}


int main()
{
	pthread_t tid;
	char *ptr="main\n";
	int ret=pthread_create(&tid,NULL,pthread_1,(void*)ptr);
	if(ret!=0)
	{
		perror("pthread_create error");
		return -1;
	}
	//char *ptr1;
	//pthread_join(tid,(void**)&ptr1);
	//printf("%s",ptr1);
	//pthread_cancel(tid);
	while(1)
	{
		printf("this is main\n");
		sleep(1);
	}
	return 0;
	
}
