#include <QDebug>
#include <QFile>
#include "../global.h"
#include "ServerUDP.h"
using namespace std;

SOCKET hSock;
SOCKADDR_IN stLclAddr, stDstAddr;
struct ip_mreq stMreq; // Multicast interface structure

/* Initialize Windows socket */
boolean ServerUDP::InitSocket(int port) {
    if (!udpConnected) {
        qDebug() << "ServerUDP:: Initializing UDP connection";
        int opt = 1;
        SOCKADDR_IN locAddr;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            qDebug() << "WSAStartup failed with error" << WSAGetLastError();
            return false;
        }

        // Get a datagram socket for listening
        if ((hSock = WSASocket(AF_INET,SOCK_DGRAM, IPPROTO_UDP, NULL, 0,
                                                 WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
            qDebug() << "WSASocket() failed with error " << WSAGetLastError();
            WSACleanup();
            return false;
        }

        // Allow socket to reuse port
        if (setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) < 0)
        {
           qDebug() << "setsockopt() SO_REUSEADDR with error " << WSAGetLastError();
           WSACleanup();
           return false;
        }

        // Name the socket (assign the local port number to receive on)
        stLclAddr.sin_family = AF_INET;
        stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY); // any address
        stLclAddr.sin_port = 0; // any port

        // Bind address to socket
        if (bind(hSock, (LPSOCKADDR)&stLclAddr, sizeof(stLclAddr)) == SOCKET_ERROR)
        {
           qDebug() << "bind() failed with error " << WSAGetLastError();
           WSACleanup();
           return false;
        }

        udpConnected = true;
        qDebug() << "UDP connection established on socket: " << hSock << " port : " << port;
    }
   return true;
}

boolean ServerUDP::MulticastSettings(const char * name)
{
    qDebug() << "In ServerUDP::MulticastSettings()";
    BOOL LoopBackFlag = false;

    //TODO replace with local ip address
    stMreq.imr_multiaddr.s_addr = inet_addr(name);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    u_long                  timeToLive = 1;

    if(setsockopt(hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on multicast address " <<  WSAGetLastError() << std::endl;
        return false;
    }

    // Set IP TTL to traverse up to multple routers
    if(setsockopt(hSock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&timeToLive, sizeof(timeToLive)) == SOCKET_ERROR)
    {
        std::cout << "setsockopt() failed with error on time to live" << WSAGetLastError() << std::endl;
        return false;
    }

    // Disable loopback
    if(setsockopt(hSock, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&LoopBackFlag, sizeof(LoopBackFlag)) == SOCKET_ERROR)
    {
        std::cout << "Setsocketopt() failed with error on loop back" << WSAGetLastError() << std::endl;
        return false;
    }

    // Assign destination address
    stDstAddr.sin_family       = AF_INET;
    stDstAddr.sin_addr.s_addr  = inet_addr(name);
    stDstAddr.sin_port         = htons(TIMECAST_PORT);

    qDebug() << "ServerUDP::MulticastSettings success";
    return true;
}

boolean ServerUDP::SendOne(LPSOCKET_INFORMATION SockInfo, char * message) {

}

// Multicast UDP send, not used (see UDPSendWorker)
boolean ServerUDP::SendAll(char * message, LPDWORD lpNumberOfBytesSent) {
    /*

   socketInformation.DataBuf.buf = message;
   socketInformation.DataBuf.len = *lpNumberOfBytesSent;

   ZeroMemory(&socketInformation.Overlapped, sizeof(WSAOVERLAPPED));
   socketInformation.Overlapped.hEvent =  WSACreateEvent();

   if (WSASendTo(socketInformation.Socket,&(socketInformation.DataBuf), 1, NULL, flags, (SOCKADDR *)&InternetAddr,
           sizeof(InternetAddr), &(socketInformation.Overlapped), NULL) < 0) {
       if (WSAGetLastError() != WSA_IO_PENDING)
       {
           qDebug() << "ServerUDP::WSASendto() failed with error " << WSAGetLastError();
           return FALSE;
       }
       if (WSAWaitForMultipleEvents(1, &socketInformation.Overlapped.hEvent, FALSE, INFINITE, FALSE) == WAIT_TIMEOUT)
       {
           qDebug() << "ServerUDP::WSASendto() Timeout";
           return FALSE;
       }
   }

   //Get the actual bytes sent.
   if(!WSAGetOverlappedResult(socketInformation.Socket, &(socketInformation.Overlapped), &socketInformation.BytesSEND, FALSE, &flags))
   {
       qDebug() << "SeverUDP::WSAGetOverlappedResult failed with errno" << WSAGetLastError();
       return FALSE;
   }
   qDebug() << "ServerUDP::Broadcast>>Bytes Sent:[" << socketInformation.BytesSEND << "]";
   */
   return true;
}

boolean ServerUDP::InitData() {
    // TODO: get next song in playlist
    QFile file("../Demo/Aero_Chord_ft_Anna_Yvette_-_Break_Them_(Monstep_Remix).wav");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ServerUDP::InitData file open fail " << file.errorString();
        return false;
    }
    qDebug() << "ServerUDP::InitData() success";
    return true;
}

void ServerUDP::CloseSocket() {
    if (udpConnected) {
        qDebug() << "Closing UDP socket...";
        multicastAddr.imr_multiaddr.s_addr = inet_addr(hostAddr);
        multicastAddr.imr_interface.s_addr = INADDR_ANY;

        /*
        if (setsockopt(hSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&multicastAddr,
                       sizeof(multicastAddr)) == SOCKET_ERROR) {
            qDebug(
                "setsockopt() IP_DROP_MEMBERSHIP address %s failed, Err: %d\n",
                hostAddr, WSAGetLastError());
        }
        */

        /* Close the socket */
        closesocket(hSock);

        /* Tell WinSock we're leaving */
        WSACleanup();

        udpConnected = false;
        qDebug() << "Closed UDP socket";
    }
}
