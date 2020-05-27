#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	while(1)
	{
		printf("Test@MiniShell]$ ");
		char buf[1024]={0};
		fgets(buf,1023,stdin);
		buf[strlen(buf)-1]='\0';

		int argc=0;
		char *argv[32]={NULL};
		char *ptr=buf;
		while(*ptr)
		{
			if(*ptr!=' ')
			{
				argv[argc++]=ptr;
				while(*ptr!=' '&&*ptr)
				{
					ptr++;	
				}
				if(*ptr=='\0')
					break;
				*ptr='\0';
			}
			ptr++;
		}
		argv[argc]=NULL;
		pid_t pid=fork();
		if(pid==0)
		{
			execvp(argv[0],argv);
			if(execvp(argv[0],argv)==-1)
			{
				perror("execvp error");
				exit(0);
			}
		}
		wait(NULL);
		
	}


	return 0;
}
