#include "TcpLoad.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include <unistd.h>
#include <iostream>

#include "Common.h"
#include "EpollHandler.h"

using namespace std;

CTcpLoad * CTcpLoad::_instance = NULL;

CTcpLoad::CTcpLoad()
{
    socketConn = 0;
    m_epoll = new CEpollHandler();
}

void CTcpLoad::DestoryInstance()
{
    if(_instance != NULL)
    {
        delete _instance;
    }
}

CTcpLoad::~CTcpLoad()
{
    
}


CTcpLoad * CTcpLoad::Instance()
{
    if(_instance == NULL)
    {
        _instance = new CTcpLoad();
    }
    
    return _instance;
}

int CTcpLoad::initTcp(int port)
{
    struct addrinfo hint, *result;
    int res;
    char strport[16];

    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;
    
    snprintf(strport, 16, "%d", port);
    res = getaddrinfo(NULL, strport, &hint, &result);
    if (res != 0) {
        cout << "error : cannot get socket address!" << endl;
        exit(1);
    }

    socketConn = socket(result->ai_family, result->ai_socktype,
                           result->ai_protocol);
    if (socketConn == -1) {
        cout << "error : cannot get socket file descriptor!" << endl;
        exit(1);
    }
    
    res = bind(socketConn, result->ai_addr, result->ai_addrlen);
    if (res == -1) {
        cout << "error : cannot bind the socket with the given address!" << endl;
        exit(1);
    }

    res = listen(socketConn, SOMAXCONN);
    if (res == -1) {
        cout << "error : cannot listen at the given socket!" << endl;
        exit(1);
    }
    //
    m_epoll->init();
    
    return 0;

}

void CTcpLoad::tcpBalance()
{
    int res;
    int comeConnect, max_sd;
    int client_sockets[MAX_SELECT_CLIENTS];
    fd_set con_sets;//select watch set
    struct sockaddr remote;
    socklen_t addrlen;
    
    memset(client_sockets, 0, sizeof(client_sockets));
    addrlen = sizeof(struct sockaddr);
    cout << "Start to Watch Connections" << endl;
    while (1) 
    {
        FD_ZERO(&con_sets);
        FD_SET(socketConn, &con_sets);
        max_sd = socketConn;
        
        int i;
        //find the max file handler
        for (i = 0; i < MAX_SELECT_CLIENTS; i++) {
            comeConnect = client_sockets[i];
          
            if (comeConnect > 0) {
                FD_SET(comeConnect, &con_sets);
            }

            if (comeConnect > max_sd) {
                max_sd = comeConnect;
            }
        }
        //Wait for the new connection
        res = select(max_sd + 1, &con_sets, NULL, NULL, NULL);
        if ( res < 0) {
            cout << "error : func select error" << endl;
            exit(1);
        }
        cout << "New Connection coming" << endl;
        if (FD_ISSET(socketConn, &con_sets)) 
        {
            comeConnect = accept(socketConn, &remote, &addrlen);
            if (comeConnect < 0) {
                cout << "error : cannot accept client socket\n" << endl;
                exit(1);
            }
            //add the new client connection to epoll watcher
            m_epoll->addHandler(comeConnect);
        }
    }
}