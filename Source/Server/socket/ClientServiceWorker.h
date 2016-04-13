#ifndef CLIENTREQUESTWORKER_H
#define CLIENTREQUESTWORKER_H
#include <QObject>
#include "Global.h"

class ClientServiceWorker : public QObject {
    Q_OBJECT
public:
    ClientServiceWorker(QString ipAddr, int socket) {
        clientIpAddr = ipAddr;
        clientSocket = socket;
    }

    ~ClientServiceWorker() {}

signals:
    void ReceivedRequest(QString);
    void ProcessedRequest();
    //void ClientDisconnected();
    //void UpdateClientList();
    void finished();


private slots:
    void ListenForRequests();
    void ProcessRequest(QString);
    //void ClientDisconnect();

private:
    QString clientIpAddr;
    int clientSocket;
    SOCKET socket;
    // boolean TransferFile();

};

#endif // CLIENTREQUESTWORKER_H
