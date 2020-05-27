#include"Tcpsocket.hpp"
#include<sys/wait.h>
void sigcb(int no)
{
	
	while(waitpid(-1,NULL,WNOHANG)>0);	
}


int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		cout<<"usage: ./serve ip port"<<endl;
		return -1;
	}
	string ip(argv[1]);
	uint16_t port=stoi(argv[2]);

	Tcpsocket srv;
	CHECK_RET(srv.Socket());
	CHECK_RET(srv.Bind(ip,port));
	CHECK_RET(srv.Listen());
	while(1)
	{
		Tcpsocket newsock;
		bool ret=srv.Accept(&newsock);
		if(ret==false)
			continue;
		pid_t pid=fork();
		if(pid==0)
		{
			while(1)
			{
				string buf;
				newsock.Recv(&buf);
				cout<<"client say: "<<buf<<endl;
				buf.clear();
				cout<<"serve say: ";
				cin>>buf;
				newsock.Send(buf);
			}
			newsock.Close();
			exit(0);
		}
		newsock.Close();
	}
	srv.Close();
	
	return 0;	
}
