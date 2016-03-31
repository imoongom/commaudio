#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtGui>
#include <QMainWindow>
#include <QFile>
#include <QButtonGroup>
#include <QPushButton>
#include <QMediaPlaylist>

#include "playback.h"
#include "recording.h"

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

private:
    Ui::MainWindow *ui;
    int icon;
    Playback *test;
    Recording *test2;
};

#endif // MAINWINDOW_H
