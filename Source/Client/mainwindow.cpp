#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList items;
    items << "Love Yourself - Justin Bieber" << "See You Again - Wiz Khalifa, Charlie Puth" << "Love Story - Taylor Swift"
          << "USA West" << "Monaco" << "Belgian" << "Spanish"
          << "Swedish" << "French" << "British" << "German"
          << "Austrian" << "Dutch" << "Italian" << "USA East"
          << "Canadian";

    QString fname = QString(":/qss_icons/rc/play-circle1.png");
    QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    QString fname3 = QString(":/qss_icons/rc/stopButton.png");

    ui->playButton->setIconSize(QSize(32, 32));
    ui->playButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->playButton->setIcon(QIcon(fname));

    ui->pauseButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->pauseButton->setIcon(QIcon(fname2));

    ui->stopButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->stopButton->setIcon(QIcon(fname3));

    ui->playList->addItems(items);

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(toggleIcon()));

}


void MainWindow::toggleIcon()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
