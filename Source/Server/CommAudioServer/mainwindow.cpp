#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket/UDPSendWorker.h"
#include "socket/ServerUDP.h"
#include "socket/TCPControlWorker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonStart_clicked() {
    qDebug() << "buttonStart server clicked";

    // TODO: get user input port
    int tcpPort = TCP_DEFAULT_PORT;

    // QThread for TCP control
    tcpControlWorkerThread = new QThread;
    tcpControlWorker = new TCPControlWorker;
    tcpControlWorker->moveToThread(tcpControlWorkerThread);

    // Start TCP control thread and connect signals & slots
    connect(tcpControlWorker, SIGNAL(SignalInitSocket(int)), tcpControlWorker, SLOT(InitSocket(int)));
    //connect(tcpControlWorker, SIGNAL(AcceptedClient(int)), tcpControlWorker, SLOT(HandleNewClient(int)));
    connect(tcpControlWorker, SIGNAL(ClosedSocket()), tcpControlWorker, SLOT(CloseSocket()));
    connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorkerThread, SLOT(quit()));
    connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorker, SLOT(deleteLater()));
    connect(tcpControlWorkerThread, SIGNAL(finished()), tcpControlWorkerThread, SLOT(deleteLater()));

    tcpControlWorkerThread->start();

    emit tcpControlWorker->SignalInitSocket(tcpPort);
    // Setup TCP
    /*
    serverTcp = new ServerTCP();
    if (!serverTcp->InitSocket(TCP_DEFAULT_PORT)){
        qDebug() << "Unable to setup tcp";
        serverTcp->CloseSocket();
        return;
    }

    while (1) {
        serverTcp->AcceptClient();
    }
    */
}

void MainWindow::on_buttonMulticast_clicked() {
    qDebug() << "buttonMulticast clicked";

    // Setup UDP
    serverUdp = new ServerUDP();
    // TODO: get user input port
    if (!serverUdp->InitSocket(TIMECAST_PORT)) {
        qDebug() << "InitSocket error, closing socket";
        serverUdp->CloseSocket();
        return;
    } else if (!serverUdp->InitData()) {
        qDebug() << "InitData error, closing socket";
        serverUdp->CloseSocket();
        return;
    } else if(!serverUdp->MulticastSettings(TIMECAST_ADDR)) {
        qDebug() << "MulticastSettings error";
        serverUdp->CloseSocket();
        return;
    }

    // QThread for sending
    udpSendWorkerThread = new QThread;
    udpSendWorker = new UDPSendWorker(serverUdp);
    udpSendWorker->moveToThread(udpSendWorkerThread);

    connect(udpSendWorkerThread, SIGNAL(started()), udpSendWorker, SLOT(Run()));
    connect(udpSendWorker, SIGNAL(SentData()), udpSendWorker, SLOT(deleteLater()));
    connect(udpSendWorkerThread, SIGNAL(finished()), udpSendWorkerThread, SLOT(deleteLater()));

    udpSendWorkerThread->start();
}
