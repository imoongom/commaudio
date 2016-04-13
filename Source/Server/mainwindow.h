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
#include "socket/UDPSendWorker.h"
#include "socket/ServerUDP.h"
#include "socket/TCPControlWorker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_playPauseButton_clicked(bool checked);
    void on_buttonTcpConnect_clicked(bool checked);
    void on_actionJoin_Multicast_triggered(bool checked);
    void HandleNewClient(int socket);

private:
    Ui::MainWindow *ui;

    bool playPauseFlag = false;

    QString fname = QString(":/qss_icons/rc/play-circle1.png");
    QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    QString fname3 = QString(":/qss_icons/rc/stopButton.png");

    ServerUDP *serverUdp;

    QThread *tcpControlWorkerThread;
    TCPControlWorker *tcpControlWorker;

    QThread *udpSendWorkerThread;
    UDPSendWorker *udpSendWorker;
};

#endif // MAINWINDOW_H
