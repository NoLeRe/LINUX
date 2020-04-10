#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/shm.h>

#define IPC_KEY 0x12345678
int main()
{
	int shmid=shmget(IPC_KEY,32,IPC_CREAT|0664);
	if(shmid<0)
	{
		perror("shmget error");
		return -1;
	}
	void *shmadrr=shmat(shmid,NULL,0);
	if(shmadrr==(void*)-1)
	{
		perror("shmat error");
		return -1;
	}
	while(1)
	{
		sleep(1);
		printf("read:[%s]\n",shmadrr);			
	}
	shmdt(shmadrr);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
	
}
