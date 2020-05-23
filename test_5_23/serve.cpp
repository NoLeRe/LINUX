#include"Udpsocket.hpp"

#define CHECK_RET(q) if((q)==false){return false;}

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage:./serve ip port"<<endl;
		return -1;
	}
	string ip(argv[1]);
	uint16_t port=stoi(argv[2]);
	
	Udpsocket srv;
	CHECK_RET(srv.Socket());
	CHECK_RET(srv.Bind(ip,port));
	while(1)
	{
		string buf;
		string peer_ip;
		uint16_t peer_port;
		CHECK_RET(srv.Recv(&buf,&peer_ip,&peer_port));
		cout<<"client["<<peer_ip<<":"<<peer_port<<"]say:"<<buf<<endl;
		buf.clear();
		cout<<"serve say:";
		cin>>buf;
		CHECK_RET(srv.Send(buf,peer_ip,peer_port));		
	}
	srv.Close();
	return 0;	
}
