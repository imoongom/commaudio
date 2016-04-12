#ifndef UDPSENDTHREAD_H
#define UDPSENDTHREAD_H

#include <QObject>
#include "ServerUDP.h"

class UDPSendThread : public QObject
{
    Q_OBJECT
public:
    UDPSendThread(ServerUDP* serverUdp);
private:
    ServerUDP *_serverUdp;

private slots:
    void run();
signals:
    void SendData();
    void StartTCP();
    void CloseSocket();
};

#endif // UDPSENDTHREAD_H
