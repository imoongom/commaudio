#include "ThreadHandler.h"
#include "ServerTCP.h"
#include "ServerUDP.h"
#include <QDebug>

ThreadHandler::ThreadHandler(QObject *parent) : QObject(parent)
{

}


DWORD WINAPI ThreadHandler::MAINThreadConnect(void * Param) {
    ThreadHandler* cthis = (ThreadHandler*)Param;
    cthis->TCPThread();
    return NULL;
}
DWORD WINAPI ThreadHandler::MAINThreadConnect2(void * Param) {
    ThreadHandler* cthis = (ThreadHandler*)Param;
    cthis->MultiThread();
    return NULL;
}

void ThreadHandler::MCreateThread() {
    qDebug() << ("ThreadHandler MCreateThread");
    HANDLE ThreadHandle, ThreadHandle2;
    HANDLE ThreadHandles[2];

    if ((ThreadHandle = CreateThread(NULL, 0, MAINThreadConnect, this, 0, NULL)) == NULL)
    {
        qDebug() << "CreateThread failed with error " << GetLastError();
        return;
    }
/*
    if ((ThreadHandle2 = CreateThread(NULL, 0, MAINThreadConnect2, this, 0, NULL)) == NULL)
    {
        printf("CreateThread failed with error %d\n", GetLastError());
        return;
    }

    ThreadHandles[0] = ThreadHandle;
    ThreadHandles[1] = ThreadHandle2;
    WaitForMultipleObjects(2, ThreadHandles, TRUE, INFINITE);

*/
    WaitForSingleObject(ThreadHandle, INFINITE);
    qDebug() << "TCP SERVER thread created";
}

//1st thread function
void ThreadHandler::TCPThread() {
    ServerTCP tcpcl;
    // TODO: get user input port
    int port = TCP_DEFAULT_PORT;
    if(tcpcl.InitSocket(port)){
        return ;
    }

   // tcpcl.MCreateThread();
}

//2nd thread function
void ThreadHandler::MultiThread() {

}
