#include"Tcpsocket.hpp"

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		cout<<"usag:./client ip port"<<endl;	
		return -1;
	}
	string srv_ip(argv[1]);
	uint16_t srv_port=stoi(argv[2]);
	
	Tcpsocket cli;
	CHECK_RET(cli.Socket());
	CHECK_RET(cli.Connect(srv_ip,srv_port));
	while(1)
	{
		string buf;
		cout<<"client say:";
		cin>>buf;
		cli.Send(buf);
		buf.clear();
		cli.Recv(&buf);
		cout<<"serve say: "<<buf<<endl;	
	}
	cli.Close();

	return 0;
}
