#include <QApplication>
#include <QDebug>
#include <QFile>
#include "UDPSendWorker.h"
#include "../Global.h"
#include "serverudp.h"
#include "circularbuffer.h"

/*---------------------------------------------------------------------------------------------------
--  SOURCE FILE:    UDPSendWorker.cpp
--  PROGRAM:        COMP 4985 : Comm Audio
--  FUNCTIONS:
--          void InitSocket(int port)
--          void SendOne(LPSOCKET_INFORMATION socketInformation, char * message)
--          boolean SendOne2(LPSOCKET_INFORMATION socketInformation, char* message)
--          void CloseSocket()

--  DATE:           April 9, 2016
--  REVISIONS:      N/A
--  DESIGNERS:      Krystle Bulalakaw
--  PROGRAMMER:     Krystle Bulalakaw
--  NOTES:
--  The worker class for a thread that handles initializing a TCP socket, listening
--  for new connections, and keeping track of connected clients.
---------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    UDPSendWorker
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     N/A
-- INTERFACE:   UDPSendWorker(ServerUDP *serverUdp)
--					ServerUDP *serverUdp - a pointer to a serverUDP object used to make calls
-- NOTES:
-- Constructor for UDPSendWorker.
-------------------------------------------------------------------------------------------------------*/
UDPSendWorker::UDPSendWorker(ServerUDP *serverUdp) :
    _serverUdp(serverUdp)
{

}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    Run
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   Run(QString songName) 
--					QString songName - name of song 
-- NOTES:
-- Opens the desired file and sends it contents to the multicast channel with completion routine.
-------------------------------------------------------------------------------------------------------*/
void UDPSendWorker::Run(QString songName) {
qDebug() << "Running UDPSendWorker";
   SOCKET_INFORMATION      SI;
   DWORD SendBytes;
   DWORD flags = 0;
   char buffer[DATA_BUFSIZE];


   QString filename = "../Demo/" + songName;

   QFile file(filename);
   if (!file.open(QIODevice::ReadOnly)) {
       qDebug() << "ServerUDP::InitData file open fail " << filename << ": " << file.errorString();
       return;
   }
   qDebug() << "ServerUDP::InitData() success";

   // Read and send until end of file
   qDebug() << "Sending " << filename << " data over UDP...";
   while (!file.atEnd() && udpConnected && songPlaying) {
       QByteArray line = file.read(DATA_BUFSIZE);
       char *sbuf = new char[DATA_BUFSIZE];
       memcpy(sbuf, line.data(), line.size());

       //initialize socket information
       SI.Socket = hSock;
       SI.DataBuf.buf = sbuf;
       SI.DataBuf.len = SendBytes;
       ZeroMemory(&(SI.Overlapped), sizeof(WSAOVERLAPPED));
       SI.Overlapped.hEvent = WSACreateEvent();

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
