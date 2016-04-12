#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtGui>
#include <QMainWindow>
#include <QFile>
#include <QButtonGroup>
#include <QPushButton>
#include <QMediaPlaylist>
#include <QDebug>

#include <QMainWindow>
#include "socket/UDPSendWorker.h"
#include "socket/TCPControlWorker.h"
#include "socket/ServerUDP.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_buttonStart_clicked();
    void on_buttonMulticast_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ServerUDP *serverUdp;

    QThread *tcpControlWorkerThread;
    TCPControlWorker *tcpControlWorker;

    QThread *udpSendWorkerThread;
    UDPSendWorker *udpSendWorker;
};

#endif // MAINWINDOW_H
