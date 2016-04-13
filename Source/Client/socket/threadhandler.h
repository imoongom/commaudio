#ifndef THREADHANDLER_H
#define THREADHANDLER_H
#include <winsock2.h>
#include <QObject>

class ThreadHandler : public QObject
{
    Q_OBJECT
public:
    explicit ThreadHandler(QObject *parent = 0);

    static DWORD WINAPI MAINThreadConnect(void * Param);
    static DWORD WINAPI MAINThreadConnect2(void * Param);


    void MultiThread();

public slots:
    void createThread();
    void TCPThread();
};


#endif // THREADHANDLER_H
