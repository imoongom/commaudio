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
    bool doneReadingFile = false;
private slots:
    void SendBufferedData(qint64 filePos, QByteArray qByteArray);
signals:
    void SentData();
    void CloseSocket();
    //void CanReadNextData(qint64 filePos);
};

#endif // UDPSENDWORKER_H
