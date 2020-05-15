#include<cstdio>
#include<iostream>
#include<pthread.h>
#include<stdlib.h>
using namespace std;


pthread_mutex_t mutex;

class lazy{
	
public:
	volatile int* getdata()
	{
		if(data==NULL)
		{
			pthread_mutex_lock(&mutex);
			if(data==NULL)
			{
				data=new int;
				*data=10;	
			}
			pthread_mutex_unlock(&mutex);
		}
		return data;
		
	}	
private:
	volatile static int *data;
};
volatile int* lazy::data=NULL;

int main()
{
	pthread_mutex_init(&mutex,NULL);
	lazy a;
	lazy b;

	printf("a:%d----%p\n",*(a.getdata()),a.getdata());
	printf("b:%d----%p\n",*(b.getdata()),b.getdata());


	pthread_mutex_destroy(&mutex);
	return 0;	
}
