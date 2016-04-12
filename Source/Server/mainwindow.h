#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>

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

private:
    Ui::MainWindow *ui;

    bool playPauseFlag = false;

    QString fname = QString(":/qss_icons/rc/play-circle1.png");
    QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    QString fname3 = QString(":/qss_icons/rc/stopButton.png");
};

#endif // MAINWINDOW_H
