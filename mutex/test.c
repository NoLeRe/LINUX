#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

int ticket=100;
pthread_mutex_t mutex;


void *pthreadfunc(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(ticket>0)
		{
			printf("you got a ticket:%d\n",ticket);
			ticket--;
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);	
		}
	}
	return NULL;
}

int main()
{
	pthread_t tid[4];
	pthread_mutex_init(&mutex,NULL);
	int i;
	for(i=0;i<4;i++)
	{
		int ret=pthread_create(&tid[i],NULL,pthreadfunc,NULL);
		if(ret!=0)
		{
			printf("pthread create error");
			return -1;
		}
	}
	for(i=0;i<4;i++)
	{
		pthread_join(tid[i],NULL);	
	}
	pthread_mutex_destroy(&mutex);
	return 0;	
}
