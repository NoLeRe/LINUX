#include<cstdio>
#include<iostream>
#include<queue>
#include<pthread.h>
using namespace std;

#define MaxQueue 5

class Blockqueue
{
public:
	Blockqueue(int maxq=MaxQueue)
	:capacity(maxq)
	{
		pthread_mutex_init(&mutex,NULL);
		pthread_cond_init(&cus_cond,NULL);
		pthread_cond_init(&pro_cond,NULL);
	}
	~Blockqueue()
	{
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cus_cond);
		pthread_cond_destroy(&pro_cond);
	}
	bool push(int data)
	{
		pthread_mutex_lock(&mutex);
		while(queue.size()==capacity)
		{
			pthread_cond_wait(&pro_cond,&mutex);	
		}
		queue.push(data);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cus_cond);
		return true;
	}
	bool pop(int *data)
	{
		pthread_mutex_lock(&mutex);
		while(queue.empty())
		{
			pthread_cond_wait(&cus_cond,&mutex);
		}
		*data=queue.front();
		queue.pop();
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&pro_cond);
		return true;
	}
	
private:
	pthread_mutex_t mutex;	 //互斥锁
	pthread_cond_t cus_cond; //消费者条件变量
	pthread_cond_t pro_cond; //生产者条件变量
	std::queue<int> queue;
	int capacity;
	
};


void *productor(void* arg)
{
	Blockqueue* q=(Blockqueue*)arg;
	int i=0;
	while(1)
	{
		q->push(i);
		printf("productor push data:%d \n",i++);
	}
	return NULL;
}
void *customer(void* arg)
{
	Blockqueue* q=(Blockqueue*)arg;
	while(1)
	{
		int ret;
		q->pop(&ret);
		printf("customer pop data:%d \n",ret);		
	}
	return NULL;
}


int main()
{
	pthread_t pro_tid[4],cus_tid[4];
	Blockqueue q;

	for(int i=0;i<4;i++)
	{
		int ret=pthread_create(&pro_tid[i],NULL,productor,(void*)&q);
		if(ret!=0)
		{
			printf("create error");
			return -1;
		}
		ret=pthread_create(&cus_tid[i],NULL,customer,(void*)&q);
		if(ret!=0)
		{
			printf("create error");
			return -1;
		}
	}

	for(int i=0;i<4;i++)
	{
		pthread_join(pro_tid[i],NULL);
		pthread_join(cus_tid[i],NULL);
	}

	return 0;
}
