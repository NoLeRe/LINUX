#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

class Udpsocket{
public:
	Udpsocket():sockfd(-1)
	{}
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
		struct sockaddr_in peer_addr;
		socklen_t len=sizeof(struct sockaddr);
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
	bool Send(string& data,string& ip, uint16_t port)
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
