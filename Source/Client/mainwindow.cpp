#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

//  ui->playList->addItems(items);

    ui->userList->addItem("Oscar");
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(toggleIcon()));

}


void MainWindow::toggleIcon()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTest_1_triggered()
{
    test = new Playback();
}

void MainWindow::on_actionPause_triggered()
{
    test->pause();
}

void MainWindow::on_actionResume_triggered()
{
    test->resume();
}

void MainWindow::on_actionTest_2_triggered()
{
    test2 = new Recording();
}

void MainWindow::on_actionPause2_triggered()
{
    test2->pause();
}

void MainWindow::on_actionPlaylist_triggered()
{
    list = new Playlist("../Demo");
    list->update_list();
    qDebug() << "updated";
    ui->playList->clear();
    ui->playList->addItems(list->get_playlist());
    qDebug() << "got list";

}

void MainWindow::on_actionRingBuf_triggered()
{
    /*
    CircularBuffer *cb;
    initBuffer(cb);
    Playback *cbtest = new Playback(cb);
    */

    Playback *pb = new Playback(new RingBuffer());

}


void MainWindow::on_actionJoin_Multicast_triggered()
{
    qDebug() << "JOIN multicast";
    ClientUDP *cl = new ClientUDP();
    cl->Start();
    cl->initData();
    cl->multiSetup();

    UDPRecvThread *multiThread = new UDPRecvThread(this);


    multiThread->start();
    connect(multiThread, SIGNAL(recvData()), cl, SLOT(writeFile()));
}
