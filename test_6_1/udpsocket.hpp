#pragma once
#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

#define CHECK_RET(q) if((q)==false){return false;}
class udpsocket{
public:
	udpsocket():sockfd(-1){}
	bool Socket()
	{
		sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(sockfd<0)
		{
			perror("socket error");
			return false;
		}
		return true;
	}
	bool Bind(string& ip,uint16_t port)
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
		struct sockaddr_in addr;
		socklen_t len=sizeof(sockaddr_in);
		char tmp[1024]={0};
		int ret=recvfrom(sockfd,tmp,1023,0,(struct sockaddr*)&addr,&len);
		if(ret<0)
		{
			perror("recv error");
			return false;
		}
		buf->assign(tmp,ret);
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
	bool Send(string& data,string& ip,uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip.c_str());
		socklen_t len=sizeof(struct sockaddr_in);
		int ret=sendto(sockfd,data.c_str(),data.size(),0,(struct sockaddr*)&addr,len);
		if(ret<0)
		{
			perror("send error");
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
