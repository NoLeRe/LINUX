#include<stdio.h>
#include<string.h>

int main()
{
	FILE* fp=NULL;
	fp=fopen("test.txt","r+");
	if(fp==NULL)
	{
		perror("fopen error");
	}

	fseek(fp,0,SEEK_END);
	char buf[]="kaman rider\n";
	int ret=fwrite(buf,strlen(buf),1,fp);
	if(ret==0)
	{
		perror("fwrite error");
		return -1;
	}
	printf("ret=%d \n",ret);
	
	fseek(fp,0,SEEK_SET);
	char temp[1024]={0};
	ret=fread(temp,1,1023,fp);
	if(ret==0)
	{
		printf("have no data or error\n");
	}
	printf("ret-[%d]--[\n%s]\n",ret,temp);


	return 0;
}
