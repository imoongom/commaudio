#include <QDebug>
#include <QFile>
#include "../Global.h"
#include "ServerUDP.h"
#include "circularbuffer.h"
using namespace std;

/*---------------------------------------------------------------------------------------
--  SOURCE FILE:    ClientServiceWorker.cpp
--  PROGRAM:        COMP 4985 : Comm Audio
--  FUNCTIONS:
--          boolean InitSocket(int port)
--          boolean MulticastSettings(const char * name)
--          void SendFileToClient(QString songName, SOCKET cSocket) 
--			boolean SendOne(LPSOCKET_INFORMATION SockInfo, char * message) 
--			boolean SendAll(char *message, LPDWORD lpNumberOfBytesSent)
--		    boolean InitData()
--			void CloseSocket()
--  DATE:           April 9, 2016
--  REVISIONS:      N/A
--  DESIGNERS:      Krystle Bulalakaw
--  PROGRAMMER:     Krystle Bulalakaw
--  NOTES:
--  The worker class for a thread that handles socket calls to create and bind a
--	UDP socket, set the local and destination address for the multicast channel used
--  to stream data, and close the socket.
---------------------------------------------------------------------------------------*/

SOCKET hSock;
SOCKADDR_IN stLclAddr, stDstAddr;
struct ip_mreq stMreq; // Multicast interface structure

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    InitSocket
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   InitSocket(int port)
-- NOTES:
-- Initializes the socket, configures port settings, configures the local address, and binds the
-- address to the socket.
-------------------------------------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    MulticastSettings
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:	MulticastSettings(const char * name)
--					const char *name - name of the socket
-- NOTES:
-- Sets the local and destination address for the multicast channel used to stream data.
-------------------------------------------------------------------------------------------------------*/
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

// Unused inherited function
boolean ServerUDP::SendOne(LPSOCKET_INFORMATION SockInfo, char * message) {

}

// Multicast UDP send, not used (see UDPSendWorker)
boolean ServerUDP::SendAll(char * message, LPDWORD lpNumberOfBytesSent) {

}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    CloseSocket
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:	CloseSocket()
-- NOTES:		Closes the TCP socket.
-------------------------------------------------------------------------------------------------------*/
void ServerUDP::CloseSocket() {
    if (udpConnected) {
        qDebug() << "Closing UDP socket...";
        multicastAddr.imr_multiaddr.s_addr = inet_addr(hostAddr);
        multicastAddr.imr_interface.s_addr = INADDR_ANY;

        /* Close the socket */
        closesocket(hSock);

        /* Tell WinSock we're leaving */
        WSACleanup();

        udpConnected = false;

        /* Clear circular buffer */
        clean_buffer(&CBuf);
        clean_buffer(&CBufSend);

        qDebug() << "Closed UDP socket";
    }
}
