#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


int bowl=0;
pthread_mutex_t mutex;
pthread_cond_t cus_cond;
pthread_cond_t cook_cond;


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
		printf("it was delicous！\n");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cook_cond);
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
		printf("it is ok\n");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cus_cond);
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

