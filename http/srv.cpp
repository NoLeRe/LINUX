#include"tcpsocket.hpp"


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage:./srv ip port"<<endl;
		return -1;
	}
	string ip(argv[1]);
	uint16_t port=stoi(argv[2]);

	tcpsocket srv;
	CHECK_RET(srv.Socket());
	CHECK_RET(srv.Bind(ip,port));
	CHECK_RET(srv.Listen());
	while(1)
	{
		tcpsocket newsock;
		bool ret=srv.Accept(&newsock);
		if(ret==false)
			continue;
		string buf;
		if(newsock.Recv(&buf)==false)
		{
			newsock.Close();
			continue;
		}
		cout<<"buf:["<<buf<<"]"<<endl;
		newsock.Close();
	}
	return 0;
	
	
}
