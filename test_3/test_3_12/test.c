#include<stdio.h>
#include<unistd.h>


int main()
{
	printf("创建进程\n");
	pid_t pid; //定义这个变量只是开辟一块空间
	pid= fork();//这个函数的返回值对于父子进程不一样

	if(pid==0)
	{
		printf("这是一个子进程\n");
	}
	//wait(NULL);
	printf("这是一个父进程\n");
	printf("进程创建完毕\n");
	return 0;
}
