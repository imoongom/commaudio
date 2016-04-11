#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket/UDPSendWorker.h"
#include "socket/ServerUDP.h"
#include "socket/ServerTCP.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->buttonStart, SIGNAL(clicked()), this, SLOT(on_buttonStart_clicked()));
    connect(ui->buttonMulticast, SIGNAL(clicked()), this, SLOT(on_buttonMulticast_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonStart_clicked() {
    qDebug() << "buttonStart server clicked";

}

void MainWindow::on_buttonMulticast_clicked() {
    qDebug() << "buttonMulticast clicked";

    // Setup TCP
    serverTcp = new ServerTCP();
    // TODO: get user input port
    if (!serverTcp->InitSocket(TCP_DEFAULT_PORT)){
        qDebug() << "Unable to setup tcp";
        return;
    }

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

    connect(udpSendWorkerThread, SIGNAL(started()), udpSendWorker, SLOT(run()));
    connect(udpSendWorker, SIGNAL(SendData()), this, SLOT(send_finished()));
    connect(udpSendWorker, SIGNAL(SendData()), udpSendWorkerThread, SLOT(quit()));
    connect(udpSendWorker, SIGNAL(SendData()), udpSendWorkerThread, SLOT(deleteLater()));
    connect(udpSendWorkerThread, SIGNAL(finished()), udpSendWorkerThread, SLOT(deleteLater()));

    udpSendWorkerThread->start();
}

void MainWindow::send_finished() {
    qDebug() << "finished send";
}
