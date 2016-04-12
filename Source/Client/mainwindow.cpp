#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initBuffer(&CBuf);

    // QString fname = QString(":/qss_icons/rc/play-circle1.png");
    //QString fname2 = QString(":/qss_icons/rc/pause-circle.png");
    // QString fname3 = QString(":/qss_icons/rc/stopButton.png");

    ui->playPauseButton->setIconSize(QSize(32, 32));
    ui->playPauseButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->playPauseButton->setIcon(QIcon(fname));

  //  ui->pauseButton->setStyleSheet("QPushButton{border: none;outline: none;}");
   // ui->pauseButton->setIcon(QIcon(fname2));

    ui->stopButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->stopButton->setIcon(QIcon(fname3));

//  ui->playList->addItems(items);

    ui->userList->addItem("Oscar");
  //  connect(ui->playPauseButton, SIGNAL(clicked()), this, SLOT(toggleIcon()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleIcon()
{

}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    test->updateVolume((float)(value / 100.0F));
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

void MainWindow::on_actionJoin_Multicast_triggered()
{
    ThreadHandler *TCPthread = new ThreadHandler();
    UDPRecvThread *multiThread = new UDPRecvThread(this);
    qDebug() << "JOIN multicast";
    ClientUDP *udp = new ClientUDP();
    if(!udp->Start() || !udp->initData() ||!udp->multiSetup()){
        udp->close();
        return ;
    }
    connect(multiThread, SIGNAL(startTCP()), TCPthread, SLOT(createThread()));
    connect(multiThread, SIGNAL(recvData()), udp, SLOT(writeFile()));
    multiThread->start();

}

void MainWindow::on_actionCB_triggered()
{

    CBufs cb;
    initBuffer(&cb);
    test = new Playback();


    QThread *t = new QThread;
    test->moveToThread(t);
    connect(t, SIGNAL(started()), test, SLOT(runthis()));
    t->start();

    wf = new WavFile();
    wf->open("../Demo/OMFG_-_Hello_(Will__Tim_Remix).wav");
}

void MainWindow::on_playPauseButton_clicked(bool checked)
{
    if (!checked) {
        ui->playPauseButton->setIcon(QIcon(fname2));
        ui->playPauseButton->setCheckable(true);
        // do pause stuff here
    } else {
        ui->playPauseButton->setIcon(QIcon(fname));
        // do Play stuff here
    }
}
