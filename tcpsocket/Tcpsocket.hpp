#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
using namespace std;

#define MAX_LISTEN 5
#define CHECK_RET(q) if((q)==false){return -1;}

class Tcpsocket{
public:
	Tcpsocket():sockfd(-1)
	{}
	bool Socket()
	{
		sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(sockfd<0)
		{
			perror("socket error");
			return false;
		}
		return true;
	}
	bool Bind(string& ip, uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip.c_str());
		socklen_t len=sizeof(struct sockaddr_in);
		int ret=bind(sockfd,(struct sockaddr*)&addr,len);
		if(ret<0)
		{
			perror("bind error");
			return false;
		}
		return true;
	}
	//进行监听，接收到连接请求后创建新的套接字
	bool Listen(int backlog=MAX_LISTEN)
	{
		int ret=listen(sockfd,backlog);
		if(ret<0)
		{
			perror("listen error");
			return false;
		}
		return true;
	}
	//获取新建套接字的操作句柄
	bool Accept(Tcpsocket* newsocket,string* ip=NULL,uint16_t* port=NULL)
	{
		struct sockaddr_in addr;
		socklen_t len=sizeof(struct sockaddr_in);
		int newfd=accept(sockfd,(struct sockaddr*)&addr,&len);
		if(newfd<0)
		{
			perror("accept error");
			return false;
		}
		newsocket->sockfd=newfd;
		if(ip!=NULL)
		{
			*ip=inet_ntoa(addr.sin_addr);	
		}
		if(port!=NULL)
		{
			*port=ntohs(addr.sin_port);	
		}
		return true;
	}
	bool Recv(string* buf)
	{
		char tmp[4096]={0};
		int ret=recv(sockfd,tmp,4096,0);
		if(ret<0)
		{
			perror("recv error");
			return false;
		}else if(ret==0)
		{
			perror("connection break");
			return false;
		}
		buf->assign(tmp,ret);
		return true;
	}
	bool Send(const string& data)
	{
		int ret=send(sockfd,data.c_str(),data.size(),0);
		if(ret<0)
		{
			perror("send error");
			return false;
		}
		return true;
	}
	bool Connect(string& ip,uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip.c_str());
		socklen_t len=sizeof(struct sockaddr_in);
		int ret=connect(sockfd,(struct sockaddr*)&addr,len);
		if(ret<0)
		{
			perror("connect error");
			return false;
		}
		return true;
			
	}
	bool Close()
	{
		if(sockfd>0)
		{
			close(sockfd);
			sockfd=-1;
		}
		return true;
	}
	
private:
	int sockfd;
};
