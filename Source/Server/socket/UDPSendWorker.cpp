#include <QApplication>
#include <QDebug>
#include <QFile>
#include "UDPSendWorker.h"
#include "../Global.h"
#include "serverudp.h"
#include "circularbuffer.h"

UDPSendWorker::UDPSendWorker(ServerUDP *serverUdp) :
    _serverUdp(serverUdp)
{

}

void UDPSendWorker::Run(){
qDebug() << "Running UDPSendWorker";
   SOCKET_INFORMATION      SI;
   DWORD SendBytes;
   DWORD flags = 0;
   char buffer[DATA_BUFSIZE];


   QString filename = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";

   QFile file(filename);
   if (!file.open(QIODevice::ReadOnly)) {
       qDebug() << "ServerUDP::InitData file open fail " << filename << ": " << file.errorString();
   }
   qDebug() << "ServerUDP::InitData() success";

   // Read and send until end of file
   qDebug() << "Sending " << filename << " data over UDP...";
   while (!file.atEnd() && udpConnected) {
       QByteArray line = file.read(DATA_BUFSIZE);
       char *sbuf = new char[DATA_BUFSIZE];
       memcpy(sbuf, line.data(), line.size());

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
       Sleep(5);
   }
   qDebug() << "UDPSendWorker::Run complete, emitting SentData signal";
   emit SentData();
}

/* Send buffered data until EOF */
/*
void UDPSendWorker::SendBufferedData(qint64 pos, QByteArray qByteArray) {
    qDebug() << "UDPSendWorker::SendBufferedData";
    SOCKET_INFORMATION SI;
    DWORD SendBytes;
    DWORD flags = 0;

    char *sbuf = new char[DATA_BUFSIZE];
    memcpy(sbuf, qByteArray.data(), qByteArray.size());
    //initialize socket information
    SI.Socket = hSock;
    SI.DataBuf.buf = sbuf;
    SI.DataBuf.len = SendBytes;
    ZeroMemory(&(SI.Overlapped), sizeof(WSAOVERLAPPED));
    SI.Overlapped.hEvent = WSACreateEvent();

    if (udpConnected && !doneReadingFile) {
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
        qDebug() << "ServerUDP::WSASendTo message sent: " << SI.DataBuf.buf;
        //Sleep(5);
        emit CanReadNextData(pos);
        qDebug() << "Done sending, can read next data";
    }
    else if (doneReadingFile) {
        //qDebug() << "UDPSendWorker::doneReadingFile, emitting SentData signal";
        emit SentData();
    }
}
*/
