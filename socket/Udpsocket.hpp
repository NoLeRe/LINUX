#include<iostream>
#include<string>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
using namespace std;

class Udpsocket{
public:
	Udpsocket():sockfd(-1)
	{}
	//创建套接字
	bool Socket()
	{
		//socket(地址域，数据类型，协议类型)
		sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(sockfd<0)
		{
			perror("socket error");
			return false;
		}
		return true;
	}
	//绑定地址
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
	bool Recv(string* buf,string* ip=NULL,uint16_t* port=NULL)
	{
		struct sockaddr_in peer_addr;
		socklen_t len=sizeof(struct sockaddr_in);
		char tmp[4096]={0};
		int ret=recvfrom(sockfd,tmp,4096,0,(struct sockaddr*)&peer_addr,&len);
		if(ret<0)
		{
			perror("recvfrom error");
			return false;
		}
		buf->assign(tmp,ret);
		if(ip!=NULL)
		{
			*ip=inet_ntoa(peer_addr.sin_addr);	
		}
		if(port!=NULL)
		{
			*port=ntohs(peer_addr.sin_port);	
		}
		return true;
		
	}
	bool Send(const string& data,const string& ip,const uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip.c_str());
		socklen_t len=sizeof(struct sockaddr_in);
		int ret=sendto(sockfd,data.c_str(),data.size(),0,(struct sockaddr*)&addr,len);
		if(ret<0)
		{
			perror("sendto error");
			return false;
		}
		return true;
	}
	bool Close()
	{
		if(sockfd!=-1)
		{
			close(sockfd);
			sockfd=-1;
		}
		return true;
	}
	
private:
	int sockfd;
};
