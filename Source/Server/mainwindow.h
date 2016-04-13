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
#include "socket/ClientServiceWorker.h"
#include "../Client/playback.h"
#include "socket/FileBufferWorker.h"

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
    void HandleNewClient(QString ipAddr, int socket);
    void PlayMusic(qint64 pos);

signals:
    void StartReadingFile(qint64 qint);
    void CanSendBufferedData(qint64, QByteArray qByteArray);

private:
    Ui::MainWindow *ui;

    bool playPauseFlag = false;

    QString fname = QString(":/qss_icons/rc/play-circle1.png");
    QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    QString fname3 = QString(":/qss_icons/rc/stopButton.png");

    ServerUDP *serverUdp;

    QThread *udpSendWorkerThread;
    QThread *tcpControlWorkerThread;
    QThread *clientServiceThread;
    QThread *playbackWorkerThread;
    QThread *fileBufferWorkerThread;

    UDPSendWorker *udpSendWorker;
    TCPControlWorker *tcpControlWorker;
    ClientServiceWorker *clientServiceWorker;
    Playback *playbackWorker;
    FileBufferWorker *fileBufferWorker;

};

#endif // MAINWINDOW_H
