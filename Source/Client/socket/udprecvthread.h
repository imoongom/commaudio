#ifndef UDPRECVTHREAD_H
#define UDPRECVTHREAD_H

#include <QThread>
#include <winsock2.h>


class UDPRecvThread : public QThread
{
    Q_OBJECT
public:
    explicit UDPRecvThread(QObject *parent =0);
private:
    void run();
signals:
    void recvData();
    void closeSock();
};

#endif // UDPRECVTHREAD_H
