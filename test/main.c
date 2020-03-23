#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

	pid_t pid=fork();
	int a=100;
	if(pid==0)
	{
		a=1;
		printf("a=%d \n",a);
		printf("Pa=%p \n",&a);
	}
	else
	{
		printf("a=%d \n",a);
		printf("Pa=%p \n",&a);
	}
	return 0;
} 
