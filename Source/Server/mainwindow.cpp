#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "circularbuffer.h"
#include "playlist.h"

bool udpConnected = false;
bool tcpConnected = false;
Playlist *playlist;

struct CBuffer CBuf, CBufSend;
struct CBuffer CBufOut;
qint64 songPos = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->playPauseButton->setIconSize(QSize(32, 32));
    ui->playPauseButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->playPauseButton->setIcon(QIcon(fname));

    ui->stopButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    ui->stopButton->setIcon(QIcon(fname3));

    initBuffer(&CBuf);
    initBuffer(&CBufSend);
    initBuffer(&CBufOut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playPauseButton_clicked(bool checked)
{
    qDebug() << "play button pressed";
    QString songName = ui->label_6->text();
    if (!checked && udpConnected && songName != "No file selected") {
        ui->playPauseButton->setIcon(QIcon(fname2));
        ui->playPauseButton->setCheckable(true);

        // QThread for sending
        udpSendWorkerThread = new QThread;
        udpSendWorker = new UDPSendWorker(serverUdp);
        udpSendWorker->moveToThread(udpSendWorkerThread);

        connect(this, SIGNAL(GotSongName(QString)), udpSendWorker, SLOT(Run(QString)));
        connect(udpSendWorker, SIGNAL(SentData()), udpSendWorker, SLOT(deleteLater()));
        connect(udpSendWorkerThread, SIGNAL(finished()), udpSendWorkerThread, SLOT(deleteLater()));

        udpSendWorkerThread->start();
        this->GotSongName(songName);
        /*
        ui->playPauseButton->setIcon(QIcon(fname2));
        ui->playPauseButton->setCheckable(true);

        //ui->playList->currentItem()->text();
        // QThread for file reading and buffering
        fileBufferWorkerThread = new QThread;
        fileBufferWorker = new FileBufferWorker();
        fileBufferWorker->moveToThread(fileBufferWorkerThread);

        // QThread for playback
        playbackWorkerThread = new QThread;
        playbackWorker = new Playback();
        playbackWorker->moveToThread(playbackWorkerThread);

        // QThread for sending
        udpSendWorkerThread = new QThread;
        udpSendWorker = new UDPSendWorker(serverUdp);
        udpSendWorker->moveToThread(udpSendWorkerThread);

        // Play music when there's data in cbuf
       connect(fileBufferWorkerThread, SIGNAL(started()), fileBufferWorker, SLOT(ReadFileAndBuffer()));
       connect(fileBufferWorker, SIGNAL(WroteToCBuf()), this, SLOT(PlayMusic()));
       //connect(playbackWorker, SIGNAL(CanSendNextData(qint64, QByteArray)), udpSendWorker, SLOT(SendBufferedData(qint64, QByteArray)));
       //connect(playbackWorker, SIGNAL(CanReadNextData(qint64)), fileBufferWorker, SLOT(ReadFileAndBuffer(qint64)));
       connect(udpSendWorker, SIGNAL(SentData()), udpSendWorker, SLOT(deleteLater()));
       connect(udpSendWorkerThread, SIGNAL(finished()), udpSendWorkerThread, SLOT(deleteLater()));

       fileBufferWorkerThread->start();
       playbackWorker->runthis();
       udpSendWorkerThread->start();
       */
    } else {
        ui->playPauseButton->setIcon(QIcon(fname));
        // do Play stuff here
    }
}

void MainWindow::on_buttonTcpConnect_clicked(bool checked) {
    if (!checked && !tcpConnected) {
        ui->buttonTcpConnect->setCheckable(true);

        // TODO: get user input port
        int tcpPort = TCP_DEFAULT_PORT;

        // QThread for TCP control
        tcpControlWorkerThread = new QThread;
        tcpControlWorker = new TCPControlWorker;
        tcpControlWorker->moveToThread(tcpControlWorkerThread);

        // Start TCP control thread and connect signals & slots
        connect(tcpControlWorker, SIGNAL(SignalInitSocket(int)), tcpControlWorker, SLOT(InitSocket(int)));
        connect(tcpControlWorker, SIGNAL(AcceptedClient(QString, int)), this, SLOT(HandleNewClient(QString, int)));
        connect(tcpControlWorker, SIGNAL(ClosedSocket()), tcpControlWorker, SLOT(CloseSocket()));
        connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorkerThread, SLOT(quit()));
        connect(tcpControlWorker, SIGNAL(finished()), tcpControlWorker, SLOT(deleteLater()));
        connect(tcpControlWorkerThread, SIGNAL(finished()), tcpControlWorkerThread, SLOT(deleteLater()));

        tcpControlWorkerThread->start();

        tcpConnected = true;
        emit tcpControlWorker->SignalInitSocket(tcpPort);
    } else if (checked && tcpConnected) {
        tcpControlWorker->finished();
        tcpControlWorker->CloseSocket();
        tcpConnected = false;
    }
}

void MainWindow::on_actionJoin_Multicast_triggered(bool checked) {
    if (!checked && !udpConnected) {
        ui->actionJoin_Multicast->setCheckable(true);
        // Setup UDP
        serverUdp = new ServerUDP();
        // TODO: get user input port
        if (!serverUdp->InitSocket(TIMECAST_PORT)) {
            qDebug() << "InitSocket error, closing socket";
            serverUdp->CloseSocket();
            return;
        } else if (!serverUdp->InitData()) {
            qDebug() << "InitData error, closing socket";
            serverUdp->CloseSocket();
            return;
        } else if(!serverUdp->MulticastSettings(TIMECAST_ADDR)) {
            qDebug() << "MulticastSettings error";
            serverUdp->CloseSocket();
            return;
        }
        udpConnected = true;
    } else if (checked && udpConnected){
        ui->playPauseButton->setIcon(QIcon(fname2));
        ui->playPauseButton->setCheckable(true);
        serverUdp->CloseSocket();
        udpConnected = false;
    }
}

/* Update the client list and start a new service thread for a new client */
void MainWindow::HandleNewClient(QString ipAddr, int socket) {
    // QThread for servicing client (receiving requests over TCP)
    clientServiceThread = new QThread;
    clientServiceWorker = new ClientServiceWorker(ipAddr, socket);
    clientServiceWorker->moveToThread(clientServiceThread);

    connect(clientServiceThread, SIGNAL(started()), clientServiceWorker, SLOT(ListenForRequests()));
    connect(clientServiceWorker, SIGNAL(ReceivedRequest(QString, int)), clientServiceWorker, SLOT(ProcessRequest(QString, int)));
    connect(clientServiceWorker, SIGNAL(finished()), clientServiceThread, SLOT(quit()));
    connect(clientServiceWorker, SIGNAL(finished()), clientServiceThread, SLOT(deleteLater()));
    connect(clientServiceThread, SIGNAL(finished()), clientServiceThread, SLOT(deleteLater()));

    clientServiceThread->start();
}

/* Audio playback of data in circular buffer */
void MainWindow::PlayMusic() {
    playbackWorker->read_data();
}


void MainWindow::on_actionAdd_Songs_triggered() {
    playlist = new Playlist("../Demo");
    playlist->update_list();
    qDebug() << "updated playlist";
    ui->playList->clear();
    ui->playList->addItems(playlist->get_playlist());
    qDebug() << "add playlist items";
}

void MainWindow::on_playList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->label_6->setText(item->text());
}
