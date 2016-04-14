#include "udprecvthread.h"
#include "../global.h"
#include "circularbuffer.h"
#include <QDebug>
#include "recording.h"

int sockPort;
Recording *gRecording;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	UDPRecvThread
--
-- DATE: 		March 25, 2016
--
-- REVISIONS:
--
-- DESIGNER	 : 	Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	UDPRecvThread
--
-- RETURNS: void
--
-- NOTES:
--	This is constructor of UDPRecvThread. initialize socket and port for UDPRecvThread class
----------------------------------------------------------------------------------------------------------------------*/
UDPRecvThread::UDPRecvThread(SOCKET sock, int port, struct CBuffer* buf, QObject *parent) : QThread(parent)
{
    hSock = sock;
    hPort = port;
    writeCBuf =buf;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	UDPClose
--
-- DATE: 		April 5, 2016
--
-- REVISIONS:
--
-- DESIGNER	 :  Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void udpConn
--
-- RETURNS: void
--
-- NOTES:
--	This function is to receive data from udp or multicast socket.
----------------------------------------------------------------------------------------------------------------------*/
 void UDPRecvThread::run(){
    qDebug()<<"Start Thread\n";
    SOCKET_INFORMATION      SI;
    SOCKADDR_IN InternetAddr;
    DWORD RecvBytes;
    DWORD flags = 0;
    int destlen;
    char buffer[DATA_BUFSIZE];

    memset(&InternetAddr, 0, sizeof(InternetAddr));
    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(hPort);//sockPort);

    qDebug()<<"[SOCKET SETUP START" << hSock;

    //initialize socket information
    SI.Overlapped.hEvent = WSACreateEvent();
    SI.Socket = hSock;
    SI.DataBuf.buf = buffer;
    SI.DataBuf.len = DATA_BUFSIZE;

    qDebug()<<"[MULTI THREAD] SOCKET NUM: "<<hSock;
    destlen = sizeof(SOCKADDR);

    //read message until timeout
    while (1) {

        memset(&SI.Overlapped, '\0', sizeof(SI.Overlapped));
        SI.Overlapped.hEvent = WSACreateEvent();

        if (WSARecvFrom(SI.Socket, &(SI.DataBuf), 1, &RecvBytes, &flags,
            (SOCKADDR *)&InternetAddr, &destlen, &SI.Overlapped, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSA_IO_PENDING)
            {
                qDebug("udp thread WSARecvFrom() failed with error %d\n", WSAGetLastError());
                emit closeSock();
                return ;
            }
            if (WSAWaitForMultipleEvents(1, &SI.Overlapped.hEvent, FALSE, 20000, FALSE) == WAIT_TIMEOUT) {
                qDebug("NONO WSAWitForMultipleEvents\n  %d", SI.Socket);
                return ;
            }

        }

        //add on Circular buffer
        write_buffer(&CBuf, SI.DataBuf.buf);
        //qDebug()<<"RECEV SUCCESS "<<hSock;
        emit recvData();

        emit SignalPlaybackVoiceData(SI.Buffer, RecvBytes);
    }
}

