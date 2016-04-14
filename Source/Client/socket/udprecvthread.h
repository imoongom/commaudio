#ifndef UDPRECVTHREAD_H
#define UDPRECVTHREAD_H

#include <QThread>
#include <winsock2.h>
#include "./global.h"

class UDPRecvThread : public QThread
{
    Q_OBJECT
public:
    explicit UDPRecvThread(SOCKET sock, int port, struct CBuffer* buf, QObject *parent =0);
    SOCKET_INFORMATION      SI;
    SOCKADDR_IN InternetAddr;
    SOCKET hSock;
    struct hostent *host;
    int hPort;
    struct CBuffer *writeCBuf;
private:
    void run();
signals:
    void recvData();
    void startTCP();
    void closeSock();
};

#endif // UDPRECVTHREAD_H
