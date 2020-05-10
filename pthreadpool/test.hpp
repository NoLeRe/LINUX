#include<cstdio>
#include<iostream>
#include<stdlib.h>
#include<pthread.h>

using namespace std;
#include<queue>

typedef void (*handler_t)(int);
class MyTask{
	
public:
	void SetTask(int data,handler_t handler)
	{
		_data=data;
		_handler=handler;
	}
	void Run()
	{
		return _handler(_data);	
	}
	
private:
	int _data;
	handler_t _handler;
};

#define Max_Thr 4
class pthreadpool{

public:
	pthreadpool(int max=Max_Thr)
	:thr_max(max)
	{
		pthread_mutex_init(&mutex,NULL);
		pthread_cond_init(&cond,NULL);
		for(int i=0;i<thr_max;i++)
		{
			pthread_t tid;
			int ret=pthread_create(&tid,NULL,thr_start,this);
			if(ret!=0)
			{
				printf("create error\n");
				exit(-1);
			}
		}
	}
	~pthreadpool()
	{
		thr_max=0;
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}
	bool taskpush(MyTask& task)
	{
		pthread_mutex_lock(&mutex);
		queue.push(task);
		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&cond);
		return true;
	}
	static void* thr_start(void* arg)
	{
		pthreadpool* p=(pthreadpool*)arg;
		while(1)
		{
			pthread_mutex_lock(&p->mutex);
			while(p->queue.empty())
			{
				pthread_cond_wait(&p->cond,&p->mutex);
			}
			MyTask task;
			task=p->queue.front();
			p->queue.pop();
			pthread_mutex_unlock(&p->mutex);
			task.Run();
		}
		return NULL;	
	}
	
	
	
private:
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int thr_max;
	std::queue<MyTask> queue;
};
