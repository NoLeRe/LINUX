#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cus_cond;
pthread_cond_t cook_cond;

int bowl=0;

void *customer(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(bowl!=1)
		{
			pthread_cond_wait(&cus_cond,&mutex);
		}

		bowl=0;
		printf("I had a bowl of rice. It was delicious\n");

		pthread_cond_signal(&cook_cond);
		pthread_mutex_unlock(&mutex);
			
	}
	return NULL;
}

void *cook(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(bowl!=0)
		{
			pthread_cond_wait(&cook_cond,&mutex);	
		}
		bowl=1;
		printf("I made a bowl of rice\n");

		pthread_cond_signal(&cus_cond);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main()
{
	pthread_t cus_tid[4],cook_tid[4];
	
	
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cus_cond,NULL);
	pthread_cond_init(&cook_cond,NULL);
	
	int i,ret;

	for(i=0;i<4;i++)
	{
		ret=pthread_create(&cus_tid[i],NULL,customer,NULL);
		if(ret!=0)
		{
			printf("create error");
			return -1;
		}
		ret=pthread_create(&cook_tid[i],NULL,cook,NULL);
		if(ret!=0)
		{
			printf("create error");
			return -1;
		}
	}
	pthread_join(cus_tid[0],NULL);
	pthread_join(cook_tid[0],NULL);


	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cus_cond);
	pthread_cond_destroy(&cook_cond);

	return 0;
	
}
