#include <QApplication>
#include "UDPSendWorker.h"
#include "../global.h"
#include "serverudp.h"
#include <QDebug>
#include <QFIle>

UDPSendWorker::UDPSendWorker(ServerUDP *serverUdp) :
    _serverUdp(serverUdp)
{

}

void UDPSendWorker::run(){
    qDebug() << "Running UDPSendWorker";
    SOCKET_INFORMATION      SI;
    DWORD SendBytes;
    DWORD flags = 0;
    char buffer[DATA_BUFSIZE];

    QFile file("../../Demo/Aero_Chord_ft_Anna_Yvette_-_Break_Them_(Monstep_Remix).wav");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ServerUDP::InitData file open fail " << file.errorString();
    }
    qDebug() << "ServerUDP::InitData() success";

    // Read and send until end of file
    while (!file.atEnd()) {
        QByteArray line = file.read(DATA_BUFSIZE);
        char *sbuf = new char[DATA_BUFSIZE];
        strcpy(sbuf, line.data()); // strcpy not best way

        //initialize socket information
        SI.Socket = hSock;
        SI.DataBuf.buf = sbuf;
        SI.DataBuf.len = SendBytes;
        ZeroMemory(&(SI.Overlapped), sizeof(WSAOVERLAPPED));
        SI.Overlapped.hEvent = WSACreateEvent();

        // UDP send without completion routine
        /*
        if (sendto(hSock, sbuf,
                   sizeof(sbuf),
                   0,
                   (SOCKADDR *)&stDstAddr,
                   sizeof(stDstAddr)) < 0) {
            qDebug() << "sendto error: " << WSAGetLastError();
        }
        */

        // With completion routine
        if (WSASendTo(SI.Socket, &(SI.DataBuf), 1, NULL, flags, (SOCKADDR *)&stDstAddr,
                sizeof(stDstAddr), &(SI.Overlapped), NULL) < 0) {
            if (WSAGetLastError() != WSA_IO_PENDING)             {
                qDebug() << "ServerUDP::WSASendto() failed with error " << WSAGetLastError();
            }

            if (WSAWaitForMultipleEvents(1, &SI.Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT) {
                qDebug() << "ServerUDP::WSASendto() Timeout";
            }
        }

        // Get bytes sent
        if(!WSAGetOverlappedResult(SI.Socket, &(SI.Overlapped), &SI.BytesSEND, FALSE, &flags)) {
            qDebug() << "SeverUDP::WSAGetOverlappedResult failed with errno" << WSAGetLastError();
        }
        qDebug() << "ServerUDP::WSASendTo Bytes Sent: " << SI.BytesSEND;
    }
    emit SendData();
}
