#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
	void *shmaddr=shmat(shmid,NULL,0);
	if(shmaddr==(void*)-1)
	{
		perror("shmat error");
		return -1;
	}
	int i=0;
	while(1)
	{
		sleep(1);
		sprintf(shmaddr,"%s+%d","hello world",i++);
	}
	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,NULL);

	return 0;	
}
