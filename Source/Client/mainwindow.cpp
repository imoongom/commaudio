#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <iostream>

ClientTCP *tcpcl;
ClientUDP *udpCl;
ClientUDP *multiCl;

SOCKET udpSock;
SOCKET multiSock;
SOCKET tcpSock;

struct CBuffer CBuf;
struct CBuffer CBufOut;

boolean _UDPconnectOn;
boolean _TCPconnectOn;
boolean _MULTIconnectOn;
boolean _VoiceChat;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initBuffer(&CBuf);
    initBuffer(&CBufOut);

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

    test = NULL;

    _UDPconnectOn = false;
    _TCPconnectOn = false;
    _MULTIconnectOn = false;
    _VoiceChat = false;
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
    if(test != NULL)
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

    if(_MULTIconnectOn)
        return;

    musicThread = new QThread();
    addPk = new Playback(&CBuf);
    qDebug() << "MULTI JOIN multicast";
    multiCl = new ClientUDP();
    if(!multiCl->Start(&multiSock, TIMECAST_PORT) || !multiCl->multiSetup(&multiSock)){
        multiCl->UDPClose();
        return ;
    }
    qDebug()<<"[MULTI] : multisetup socket opened "<<multiSock;
    _MULTIconnectOn = true;
    multiThread = new UDPRecvThread(multiSock, TIMECAST_PORT, this);
    addPk->moveToThread(musicThread);


    connect(musicThread, SIGNAL(started()), addPk, SLOT(runthis()));
    connect(multiThread, SIGNAL(recvData()), this, SLOT(appendMusicPk()));

    multiThread->start();
    musicThread->start();


    qDebug()<<"[MULTI] THREAD STARTED "<<multiSock;


}

void MainWindow::udpRecvSetup(){
    if(_UDPconnectOn)
        return;

    voiceThread = new QThread();
    addVoice= new Playback(&CBufOut);
    qDebug() << "UDP JOIN";
    udpCl = new ClientUDP();
    if(!udpCl->Start(&udpSock,UDP_DEFAULT_PORT)){
        udpCl->UDPClose();
        return ;
    }
    qDebug()<<"[UDP] : socket opened "<<udpSock;
    _UDPconnectOn = true;
    addVoice->moveToThread(voiceThread);
    udpThread = new UDPRecvThread(udpSock, UDP_DEFAULT_PORT, this);

    connect(voiceThread, SIGNAL(started()), addVoice, SLOT(runthis()));
    connect(udpThread, SIGNAL(recvData()), this, SLOT(testVoiceRecv()));

    udpThread->start();
    voiceThread->start();


    qDebug()<<"[UDP] THREAD STARTED "<<udpSock;
}

void MainWindow::testVoiceRecv(){
    qDebug("VoiceReceived");
    addVoice->read_data();
}


void MainWindow::on_actionCB_triggered()
{

    CBufs cb;
    initBuffer(&cb);
    test = new Playback(&CBufOut);
    test2 = new Recording();


    QThread *t = new QThread;
    test->moveToThread(t);
    connect(t, SIGNAL(started()), test, SLOT(runthis()));
    t->start();

    QThread *t2 = new QThread;
    test2->moveToThread(t2);
    connect(t2, SIGNAL(started()), test2, SLOT(runthis()));
    t2->start();

//    test->write_data();
//    test->read_data();
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

void MainWindow::on_connectButton_clicked()
{
    if(_TCPconnectOn){
       return;
    }

    //get input value
    QString host_ip_addr = ui->lineEdit_ip->text();
    int host_port_no = ui->lineEdit_port->text().toInt();

    TCPThread = new QThread();

    ThreadHandler *TCPhandler = new ThreadHandler();

    //initialize tcp and udp
    if(host_ip_addr.size()==0 && host_port_no == 0)
        tcpcl = new ClientTCP();
    else
        tcpcl = new ClientTCP(host_ip_addr.toStdString(), host_port_no,this);

    if(!_MULTIconnectOn){
        qDebug()<<"???MULTI cnnect call";
        on_actionJoin_Multicast_triggered();
    }
    qDebug()<<"###Multi connect success :" << multiSock;

    if(!_UDPconnectOn){
        qDebug()<<"???UDP cnnect call";
        udpRecvSetup();
    }
    qDebug()<<"###UDP Connect success :" << udpSock;
    connect(TCPThread, SIGNAL(started()), TCPhandler, SLOT(TCPThread()));

    TCPhandler->moveToThread(TCPThread);
    TCPThread->start();

}

//dummy to make space circularbuffer
void MainWindow::appendMusicPk(){
    addPk->read_data();
 //   char buffer[CIRBUFSIZE];
//    if(CBuf._count ==0)
//        return;
//    read_buffer(&CBuf, &buffer);
//    qDebug("CBUFFER readed");
//    addPk->moveToThread(musicThread);
//    musicThread->start();



}

void MainWindow::on_actionRecording_triggered()
{
//    test2->record();
    test->read_data();
//    delete test2;
}

void MainWindow::on_pushToTalk_clicked(bool checked)
{    

    qDebug()<<"Talk Pushed";

    ClientUDP * speaker = new ClientUDP();

    if (!checked) {
        ui->pushToTalk->setStyleSheet("background-color:#454389");
        ui->pushToTalk->setCheckable(true);

        QThread *temp = new QThread();

         _VoiceChat = TRUE;
        speaker->moveToThread(temp);
        connect(temp, SIGNAL(started()), speaker, SLOT(voiceStart()));
        connect(temp, SIGNAL(voiceGo(char* addr)), speaker, SLOT(sendVoice(char *ip)));
        temp->start();
        temp_add_music();


        // toggled
    } else {
        // not toggled
        _VoiceChat = FALSE;
        speaker->UDPClose();
        ui->pushToTalk->setStyleSheet("background-color:#524FA1");

    }
}

void MainWindow::temp_add_music(){
    QString filename = "../ChrisBrown-WithYou.wav";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "[ServerUDP::InitData _temp_add_file open fail " << filename << ": " << file.errorString();
    }
    qDebug() << "temp_add_music";

    // Read and send until end of file
    qDebug() << "ADD instead of VOICE " << filename ;
    while (!file.atEnd() ){//&& _VoiceChat) {
        qDebug()<<"Read! file " << filename;
        QByteArray line = file.read(DATA_BUFSIZE);
        char *sbuf = new char[DATA_BUFSIZE];
        memcpy(sbuf, line.data(), line.size());
        write_buffer(&CBufOut, sbuf);
    }

}

void MainWindow::on_pushButton_pressed()
{
    ui->pushButton->setStyleSheet("background-color:#454389");
}

void MainWindow::on_pushButton_released()
{
    ui->pushButton->setStyleSheet("background-color:#524FA1;color:white;");
    QString temp = ui->playList->currentItem()->text();

    // get name of file, request to server, server sends back file
}
