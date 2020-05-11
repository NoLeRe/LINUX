#include<cstdio>
#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<queue>
using namespace std;


typedef void(*handler_t)(int data);  //定义一个任务处理函数指针
class Task{
	
public:
	//进行数据的传入，处理函数的传入
	void settask(int data,handler_t handler)
	{
		_data=data;
		_handler=handler;
	}
	//进行任务处理
	void Run()
	{
		_handler(_data);	
	}
	
private:
	int _data;
	handler_t _handler;
};

//线程池
#define Max_pthread 4
class pthreadpool{
	
public:
	pthreadpool(int max=Max_pthread)
	:_max(max)
	{
		pthread_mutex_init(&mutex,NULL);
		pthread_cond_init(&cond,NULL);
		for(int i=0;i<_max;i++)
		{
			pthread_t pthread;
			int ret=pthread_create(&pthread,NULL,thr_start,this);
			if(ret!=0)
			{
				printf("create error\n");
				exit(-1);
			}
		}
	}
	~pthreadpool()
	{
		_max=0;
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}
	bool taskpush(Task& task)
	{
		pthread_mutex_lock(&mutex);
		queue.push(task);
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&cond);
		return true;
	}
	static void* thr_start(void* arg)
	{
			pthread_cond_broadcast(&p->cond);//唤醒所有线程，让他们随机去获取这个任务
		pthreadpool* p=(pthreadpool*)arg;
		while(1)
		{
			//对从任务队列中获取任务操作进行上锁，保证线程安全
			pthread_mutex_lock(&p->mutex);
			while(p->queue.empty())    //任务队列如果为空，进行线程等待
			{
				pthread_cond_wait(&p->cond,&p->mutex);	
			}
			Task task;
			task=p->queue.front();
			p->queue.pop();
			pthread_mutex_unlock(&p->mutex);//获取完任务后进行解锁
			task.Run();
		}
		
	}


private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	std::queue<Task> queue;
	int _max;
};
