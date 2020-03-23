#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
	
	while(1)
	{
		//shell终端名称
		printf("[Test@MiniShell]$ ");
		//输入命令
		char Buf[1024]={0};
		fgets(Buf,1023,stdin);
		Buf[strlen(Buf)-1]='\0';

		//对数据进行解析
		char *argv[32]={NULL};
		int argc=0;
		char *ptr=Buf;
		while(*ptr!='\0')
		{
			if(*ptr!=' ')
			{
				argv[argc++]=ptr;
				while(*ptr!=' '&&*ptr!='\0')
					ptr++;
				if(*ptr=='\0')
					break;
				*ptr='\0';
			}
			ptr++;
		}
		argv[argc]=NULL;
		//创建子进程进行程序替换
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
