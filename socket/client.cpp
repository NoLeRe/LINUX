#include"Udpsocket.hpp"


#define CHECK_RET(q) if((q)==false){return false;}


int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage: ./client ip port"<<endl;
		return -1;
	}
	
	string srv_ip(argv[1]);
	uint16_t srv_port=stoi(argv[2]);

	Udpsocket client;
	//创建套接字
	CHECK_RET(client.Socket());

	while(1)
	{
		//发送数据
		string buf;
		cout<<"client says:";
		cin>>buf;
		CHECK_RET(client.Send(buf,srv_ip,srv_port));
		//接收数据
		buf.clear();
		CHECK_RET(client.Recv(&buf));
		cout<<"server says:"<<buf<<endl;
	}
	client.Close();
	
	
	return 0;	
}
