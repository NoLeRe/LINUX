#include<cstdio>
#include<iostream>
using namespace std;

#include<pthread.h>


pthread_mutex_t mutex;
class single_instance{
	
public:
	volatile int* get_instance()
	{
		if(_data==NULL)
		{
			pthread_mutex_lock(&mutex);
			if(_data==NULL)
			{
				_data=new int;
				*_data=10;
			}
			pthread_mutex_unlock(&mutex);
		}
		return _data;
		
	}


private:
	volatile static int *_data;
};

volatile int*  single_instance:: _data=NULL;

int main()
{
	pthread_mutex_init(&mutex,NULL);
	single_instance a;
	single_instance b;

	printf("a:%d-%p\n", *(a.get_instance()), a.get_instance()); 
	printf("b:%d-%p\n", *(b.get_instance()), b.get_instance());
	
	pthread_mutex_destroy(&mutex);
	return 0;
}

