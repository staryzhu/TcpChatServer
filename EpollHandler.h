#ifndef EPOLLHANDLE

#define EPOLLHANDLE

class CEpollHandler
{
public:
    CEpollHandler();
    ~CEpollHandler();
    int init();
    int addHandler(int nHandler);
    
private:
    int m_epoll_handler;
    
};

#endif //EPOLLHANDLE