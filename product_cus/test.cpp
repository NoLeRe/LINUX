#include<iostream>
#include<cstdio>
#include<queue>
#include<pthread.h>
using namespace std;

#define MAX_QUEUE 5

class BlockQueue
{
public:
    BlockQueue(int capacity=MAX_QUEUE)
    :_capacity(capacity)
    {
        pthread_cond_init(&cus_cond,NULL);
        pthread_cond_init(&pro_cond,NULL);
        pthread_mutex_init(&_mutex,NULL);
    }
    ~BlockQueue()
    {
        pthread_cond_destroy(&cus_cond);
        pthread_cond_destroy(&pro_cond);
        pthread_mutex_destroy(&_mutex);
    }
    bool push(int data)
    {
        pthread_mutex_lock(&_mutex);
        while(q.size()==_capacity)
        {
            pthread_cond_wait(&pro_cond,&_mutex);
        }
        q.push(data);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&cus_cond);
        return true;
    }
    bool pop(int* data)
    {
        pthread_mutex_lock(&_mutex);
        while(q.size()==0)
        {
            pthread_cond_wait(&cus_cond,&_mutex);
        }
        *data=q.front();
        q.pop();
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&pro_cond);
        return true;
    }

private:
    pthread_cond_t cus_cond;
    pthread_cond_t pro_cond;
    pthread_mutex_t _mutex;
    queue<int> q;
    int _capacity;
};

void* productor(void* arg)
{
    BlockQueue* q=(BlockQueue*)arg;
    int i=0;
    while(1)
    {
        q->push(i);
        printf("productor push data:%d \n",i++);
    }
    return NULL;
}
void* customer(void* arg)
{
    BlockQueue* q=(BlockQueue*)arg;
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
    pthread_t pro[4], cus[4];
    BlockQueue q;

    for(int i=0;i<4;i++)
    {
        int ret=pthread_create(&pro[i],NULL,productor,(void*)&q);
        if(ret!=0)
        {
            printf("create error\n");
            return -1;
        }
        ret=pthread_create(&cus[i],NULL,customer,(void*)&q);
        if(ret!=0)
        {
            printf("create error\n");
            return -1;
        }
    }
    for(int i=0;i<4;i++)
    {
        pthread_join(pro[i],NULL);
        pthread_join(cus[i],NULL);
    }
    return 0;
}
