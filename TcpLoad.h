#ifndef TCPLOAD
#define TCPLOAD

#include "EpollHandler.h"

class CTcpLoad
{
public:
    static CTcpLoad * Instance();
    static void DestoryInstance();
    
    int initTcp(int port);
    
    void tcpBalance();
    
private:
    CTcpLoad();
    ~CTcpLoad();
    
    static CTcpLoad * _instance;
    int socketConn;
    CEpollHandler * m_epoll;
};

#endif // TCPLOAD