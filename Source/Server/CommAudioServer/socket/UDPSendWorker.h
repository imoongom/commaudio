#ifndef UDPSENDWORKER_H
#define UDPSENDWORKER_H

#include <QObject>
#include "ServerUDP.h"

class UDPSendWorker : public QObject
{
    Q_OBJECT
public:
    UDPSendWorker(ServerUDP* serverUdp);
private:
    ServerUDP *_serverUdp;

private slots:
    void run();
signals:
    void SendData();
    void StartTCP();
    void CloseSocket();
};

#endif // UDPSENDWORKER_H
