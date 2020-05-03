#include<cstdio>
#include<iostream>
#include<vector>
#include<semaphore.h>
#include<pthread.h>
using namespace std;

#define Maxqueue 5

class Blockqueue{
public:	
	Blockqueue(int maxq=Maxqueue)
	:queue(maxq)
	,capacity(maxq)
	,read(0)
	,write(0)
	{
		sem_init(&lock,0,1);
		sem_init(&pro,0,maxq);
		sem_init(&cus,0,0);
		
	}
	~Blockqueue()
	{
		sem_destroy(&lock);
		sem_destroy(&pro);
		sem_destroy(&cus);
	}
	bool push(int data)
	{
		//判读是否有空间可以加入数据，不满足则阻塞等待
		sem_wait(&pro);
		//满足条件则上锁
		sem_wait(&lock);
		//添加数据
		queue[write]=data;
		write=(write+1)%capacity;
		sem_post(&lock);
		sem_post(&cus);
		return true;
	}
	bool pop(int *data)
	{
		sem_wait(&cus);
		sem_wait(&lock);
		*data=queue[read];
		read=(read+1)%capacity;
		sem_post(&lock);
		sem_post(&pro);
		return true;
	}
	
private:
	vector<int> queue;
	
	int capacity;
	int read;
	int write;

	sem_t lock;
	sem_t pro;
	sem_t cus;
};

void* productor(void* arg)
{
	Blockqueue* q=(Blockqueue*)arg;
	int i=0;
	while(1)
	{
		q->push(i);
		printf("The product data is:%d \n",i++);
	}
	return NULL;
}
void* customer(void* arg)
{
	Blockqueue* q=(Blockqueue*)arg;
	while(1)
	{
		int data;
		q->pop(&data);
		printf("The custome data is:%d \n",data);
		
	}
	return NULL;
}

int main()
{
	Blockqueue q;
	pthread_t p_tid[4],c_tid[4];

	for(int i=0;i<4;i++)
	{
		int ret=pthread_create(&p_tid[i],NULL,productor,(void*)&q);
		if(ret!=0)
		{
			printf("create error\n");
			return -1;
		}
		ret=pthread_create(&c_tid[i],NULL,customer,(void*)&q);
		if(ret!=0)
		{
			printf("create error\n");
			return -1;		
		}
	}
	for(int i=0;i<4;i++)
	{
		pthread_join(p_tid[i],NULL);
		pthread_join(c_tid[i],NULL);
	}
	
	return 0;	
}
