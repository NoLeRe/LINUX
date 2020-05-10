#include"test.hpp"
#include<unistd.h>


void func1(int data)
{
	int sec=(data%3)+1;
	printf("tid:%p --- get data:%d----sleep:%d \n",pthread_self(),data,sec);
	sleep(sec);
}

void func2(int data)
{
	printf("tid:%p -----func2\n",pthread_self());
	sleep(1);
	
}



int main()
{
	
	pthreadpool pool;
	for(int i=0;i<10;i++)
	{
		MyTask task;
		if(i%2==0)
			task.SetTask(i,func1);
		else
			task.SetTask(i,func2);
		pool.taskpush(task);
	}

	sleep(1000);
	return 0;	
}
