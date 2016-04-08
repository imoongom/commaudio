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

#include "playback.h"
#include "recording.h"
#include "playlist.h"
#include "socket/clientudp.h"
#include "socket/udprecvthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toggleIcon();

private slots:
    void on_actionTest_1_triggered();

    void on_actionPause_triggered();

    void on_actionResume_triggered();

    void on_actionTest_2_triggered();

    void on_actionPause2_triggered();

    void on_actionPlaylist_triggered();

    void on_actionJoin_Multicast_triggered();

private:
    Ui::MainWindow *ui;
    int icon;
    Playback *test;
    Recording *test2;
    Playlist *list;
};

#endif // MAINWINDOW_H
