#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "udprecvthread.h"
#include "circularbuffer.h"


#define WORD unsigned short
#define MAXADDRSTR  16
#define TIMECAST_ADDR "234.5.6.7"


class ClientUDP :public QObject{
    Q_OBJECT
public:

    int Start();
    void multiSetup();
    void initData();
    int close();



    SOCKET hSocket;
    char hostAddr[MAXADDRSTR] = TIMECAST_ADDR;
    u_long lhostAddr;
    u_short nPort;// = TIMECAST_PORT;
    FILE *fstream;


    struct ip_mreq stMreq;         /* Multicast interface structure */

public slots:
    void writeFile();
};


#endif // CLIENTUDP_H
