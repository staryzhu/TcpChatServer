#include <iostream>

#include "TcpLoad.h"

using namespace std;


int main(int argc, char * argv[])
{
    CTcpLoad * tcp = CTcpLoad::Instance();
    tcp->initTcp(19999);
    tcp->tcpBalance();
    
    return 0;
}