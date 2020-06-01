#include"udpsocket.hpp"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage:./client ip port"<<endl;
		return -1;
	}
	string ip(argv[1]);
	uint16_t port=stoi(argv[2]);

	udpsocket cli;
	CHECK_RET(cli.Socket());

	while(1)
	{
		string buf;
		cout<<"client:";
		getline(cin,buf);

		CHECK_RET(cli.Send(buf,ip,port));
		buf.clear();
		CHECK_RET(cli.Recv(&buf));
		cout<<"serve:"<<buf<<endl;
	}
	cli.Close();
	return 0;
	
}
