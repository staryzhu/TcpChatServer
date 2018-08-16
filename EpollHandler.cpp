#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#include "EpollHandler.h"
#include "Common.h"

CEpollHandler::CEpollHandler()
{
    m_epoll_handler = 0;
}

CEpollHandler::~CEpollHandler()
{
    
    
}

void *work(void *param)
{
    //int flags, res;
    int handler;
    struct epoll_event events[EPOLL_EVENTS_MAX];
    handler = *((int *)param);
    cout << "init Thread and start work" << endl;
    while (1) {
        int cnt = epoll_wait(handler, events, EPOLL_EVENTS_MAX, -1);
        
        for (int i = 0; i < cnt; i++) {
            
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP))
            {
                cout << "error : socket fd error!\n" << endl;
                close(events[i].data.fd);
                continue;
            
            } 
            else if (events[i].events & EPOLLIN) 
            {
                int cnt;
                char buf[1024];
                
                do{
                    cnt = read(events[i].data.fd,  buf, 1024);
                    if (cnt == -1) 
                    {
                        if (errno == EAGAIN) {
                            break;
                        }
                        
                        cout << "error : read error!\n" << endl;
                        //return NULL;
                        break;
                
                    } else if (cnt == 0) {
                        close(events[i].data.fd);
                        cout << "cnt : 0 close this fd" << endl;
                        break;
                        //return NULL;
                    } 
                
                    cout << "receive client data : " << buf << endl;
                    struct epoll_event ev;
                    ev.events = EPOLLOUT;
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(handler,EPOLL_CTL_MOD,events[i].data.fd,&ev);

                    
                }while(0);
            }
            else if (events[i].events & EPOLLOUT)
            {
                int nwrite;
                static int count = 0;
                char buf[128];
                snprintf(buf, 128, "send msg success : %d", count);
                count++;
                cout << "echo back msg" << endl;
                nwrite = write(events[i].data.fd,buf,strlen(buf));
                if (nwrite == -1)
                {
                    cout << "write error:" << endl;
                    close(events[i].data.fd);
                }
                else
                {
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(handler,EPOLL_CTL_MOD,events[i].data.fd,&ev);
                }
            }
            else 
            {
                cout << "Do nothing" << endl;
            }
        }
    }
    return NULL;
}

int CEpollHandler::init()
{
    //int epoll;
    m_epoll_handler = epoll_create(EPOLL_WATCH_SIZE);
    if (m_epoll_handler == -1) 
    {
        cout << "error : cannot create epoll!" << endl;
        return -1;
    }
    pthread_t id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    pthread_create(&id, &attr, &work, (void *)&m_epoll_handler);
    //销毁线程属性结构,它在重新初始化之前不能重新使用
    pthread_attr_destroy(&attr);

    return 0;
}


int CEpollHandler::addHandler(int nHandler)
{
    int flags, res;
    struct epoll_event event, events[EPOLL_EVENTS_MAX];
    
    if(m_epoll_handler == 0)
    {
        cout << "epoll not initialize" << endl;
        return -1;
    }
    
    flags = fcntl(nHandler, F_GETFL);
    if (flags == -1) {
        cout << "error : cannot get socket flags!" << endl;
        return -1;
    }
    flags |= O_NONBLOCK;
    res    = fcntl(nHandler, F_SETFL, flags);
    if (res == -1) {
        cout << "error : cannot set socket flags!" << endl;
        return -1;
    }
    
    
    event.events  = EPOLLIN | EPOLLET;
    event.data.fd = nHandler;
    res = epoll_ctl(m_epoll_handler, EPOLL_CTL_ADD, nHandler, &event);
    if (res == -1) {
        cout << "error : can not add event to epoll!\n" << endl;
        return -1;
    }
    
    
    //close(nHandler);
    return 0;
}