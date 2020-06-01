#include"udpsocket.hpp"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage:./serve ip port"<<endl;
		return -1;
	}
	string ip(argv[1]);
	uint16_t port=stoi(argv[2]);

	udpsocket srv;
	CHECK_RET(srv.Socket());//创建套接字
	CHECK_RET(srv.Bind(ip,port));//绑定服务器地址

	while(1)
	{
		string buf;
		string cli_ip;
		uint16_t cli_port;
		CHECK_RET(srv.Recv(&buf,&cli_ip,&cli_port));
		cout<<"client:"<<buf<<endl;
		buf.clear();
		cout<<"serve:";
		getline(cin,buf);
		CHECK_RET(srv.Send(buf,cli_ip,cli_port));
	}
	srv.Close();
	return 0;
}
