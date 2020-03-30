#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	while(1)
	{   
		//接收输入
		printf("[test@minishell]$ ");
		char buf[1024]={0};
		fgets(buf,1023,stdin);
		buf[strlen(buf)-1]='\0';

		//解析输入
		char *ptr=buf;

		//重定向
		int flag=0;
		char *fp=NULL;
		while(*ptr)
		{
			if(*ptr=='>')
			{
				flag=1;
				*ptr='\0';
				ptr++;
				if(*ptr=='>')
				{
					flag=2;
					*ptr='\0';
					ptr++;
				}
				while(*ptr==' '&&*ptr)
					ptr++;
				fp=ptr;
				while(*ptr!=' '&&ptr)
					ptr++;
				if(*ptr=='\0')
					break;
				*ptr='\0';
			}
			ptr++;	
		}

		ptr=buf;
		int argc=0;
		char *argv[1024]={NULL};
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

	    //创建子进程进行进程替换
		pid_t pid=fork();
		if(pid==0)
		{
			if(flag==1)
			{
				int fd=open(fp,O_RDWR|O_CREAT|O_TRUNC,0664);
				dup2(fd,1);
			}
			else if(flag==2)
			{
				int fd=open(fp,O_RDWR|O_CREAT|O_APPEND,0664);
				dup2(fd,1);
			}
			execvp(argv[0],argv);
			perror("execvp error");
			exit(0);
		}
		wait(NULL);
	}
	return 0;
}
