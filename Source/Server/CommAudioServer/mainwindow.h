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
#include "socket/UDPSendThread.h"
#include "socket/ServerTCP.h"
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
    void send_finished();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ServerUDP *serverUdp;
    ServerTCP *serverTcp;

    QThread *udpSendWorkerThread;
    UDPSendThread *udpSendWorker;

};

#endif // MAINWINDOW_H
