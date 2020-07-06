#include<iostream>
#include<unistd.h>
#include<sys/epoll.h>
#include<vector>

class Epoll{
public:
    Epoll()
    {
        _epfd=epoll_create(1);
        if(_epfd<0)
        {
            perror("epoll create error");
            _exit(0);
        }
    }
    bool Add(tcpsocket& sock)
    {
        int fd=sock.Getfd();
        struct epoll_event ev;
        ev.data.fd=fd;
        ev.events=EPOLLIN;
        int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
        if(ret<0)
        {
            perror("epoll_ctl add error");
            return false;
        }
        return true;
    }
    bool Del(tcpsocket& sock)
    {
        int fd=sock.Getfd();
        int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,NULL);
        if(ret<0)
        {
            perror("epoll_ctl del error");
            return false;
        }
        return true;
    }
    bool Wait(std::vector<tcpsocket>* list,int timeout=3000)
    {
        struct epoll_event evs[10];
        int ret=epoll_wait(_epfd,evs,10,timeout);
        if(ret<0)
        {
            perror("epoll_wait error");
            return false;
        }
        else if(ret==0)
        {
            std::cout<<"epoll_wait time out"<<std::endl;
            return false;
        }
        for(int i=0;i<ret;i++)
        {
            if(evs[i].events&EPOLLIN)
            {
                tcpsocket sock;
                sock.Setfd(evs[i].data.fd);
                list->push_back(sock);

            }
        }
        return true;

    }

private:
    int _epfd;
};

