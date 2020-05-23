#include"Udpsocket.hpp"

#define CHECK_RET(q) if((q)==false){return false;}

int main(int argc,char* argv[])
{	
	if(argc!=3)
	{
		cout<<"usage: ./client ip .port"<<endl;
		return -1;
	}

	string peer_ip=argv[1];
	uint16_t peer_port=stoi(argv[2]);

	Udpsocket cli;
	CHECK_RET(cli.Socket());

	while(1)
	{
		cout<<"client say: ";
		string buf;
		cin>>buf;
		CHECK_RET(cli.Send(buf,peer_ip,peer_port));
		buf.clear();
		CHECK_RET(cli.Recv(&buf));
		cout<<"serve say: "<<buf<<endl;
	}
	cli.Close();
		
	return 0;
}
