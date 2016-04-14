#include "threadhandler.h"
#include "clienttcp.h"
#include "clientudp.h"
#include <QDebug>

extern ClientTCP *tcpcl;
extern ClientUDP *udpCl;
extern boolean _TCPconnectOn;
ThreadHandler::ThreadHandler(QObject *parent) : QObject(parent)
{

}

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

DWORD WINAPI ThreadHandler::MAINThreadConnect(void * Param) {
    ThreadHandler* cthis = (ThreadHandler*)Param;
    cthis->TCPThread();
    return NULL;
}

void ThreadHandler::createThread() {
    printf("### client main create Thread\n");
    HANDLE ThreadHandle, ThreadHandle2;
    HANDLE ThreadHandles[2];

    if ((ThreadHandle = CreateThread(NULL, 0, MAINThreadConnect, this, 0, NULL)) == NULL)
    {
        printf("CreateThread failed with error %d\n", GetLastError());
        return;
    }

    WaitForSingleObject(ThreadHandle, 2000);
    printf("### TCP SERVER thread CREATED!!!!!!!!\n");
}

//1st thread function
void ThreadHandler::TCPThread() {
 //   ClientTCP tcpcl;
    if(!tcpcl->TCPConnect()){
        qDebug()<<"TCP Connection fail";
        _TCPconnectOn = false;
        return ;
    }
    _TCPconnectOn = true;
    tcpcl->TCPcreateThread();
    qDebug()<< "TCP creat Thread";
}

//2nd thread function
void ThreadHandler::MultiThread() {
    ClientUDP cl;


}
