#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "circularbuffer.h"
#include "playlist.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: MainWindow.cpp - An application that will stream music to other clients.
--
-- PROGRAM: COMP 4985 : Comm Audio
--
-- FUNCTIONS:
-- explicit MainWindow(QWidget *parent = 0)
-- ~MainWindow()
-- void on_playPauseButton_clicked(bool checked)
-- void on_buttonTcpConnect_clicked(bool checked)
-- void on_actionJoin_Multicast_triggered(bool checked)
-- void on_actionAdd_Songs_triggered()
-- void on_playList_itemDoubleClicked(QListWidgetItem *item)
-- void HandleNewClient(QString ipAddr, int socket)
-- void PlayMusic()
-- DATE: April 14, 2016
-- REVISIONS: (Date and Description)
-- DESIGNER: Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
-- PROGRAMMER: Krystle Bulalakaw
-- NOTES:
-- The main GUI window that the user will interact to perform all functions of the application.
----------------------------------------------------------------------------------------------------------------------*/

bool udpConnected = false;
bool tcpConnected = false;
bool songPlaying = false;
Playlist *playlist;

struct CBuffer CBuf, CBufSend;
struct CBuffer CBufOut;

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    MainWindow
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     N/A
-- INTERFACE:   N/A
-- NOTES:
-- Constructor for MainWindow. Stylizes some buttons and initializes the circular buffers.
------------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    ~MainWindow
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     N/A
-- INTERFACE:   N/A
-- NOTES:
-- Destructor for MainWindow.
------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    on_playPauseButton_clicked
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   bool checked - is the button checked
-- NOTES:
-- When the play/pause button is clicked, toggle the songPlaying status. If no UDP connection is established,
-- or no song is selected, do not do anything. If a UDP connection is established, open the currently 
-- song, read and send its contents to the multicast chanel. Else if a song is already playing, end the
-- thread that is streaming data.
------------------------------------------------------------------------------------------------------------*/

void MainWindow::on_playPauseButton_clicked(bool checked)
{
    qDebug() << "play button pressed";
    QString songName = ui->label_6->text();
    if (!songPlaying) {
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
            connect(this, SIGNAL(EndCurrentlyPlaying()), udpSendWorker, SLOT(deleteLater()));
            connect(this, SIGNAL(EndCurrentlyPlaying()), udpSendWorkerThread, SLOT(deleteLater()));

            udpSendWorkerThread->start();
            this->GotSongName(songName);

            songPlaying = true;
        } else {
            // suspend udpSendThread?
            udpSendWorkerThread->terminate();
        }
    } else {
        ui->playPauseButton->setIcon(QIcon(fname));
        // do Play stuff here
        songPlaying = false;

    }
}

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    on_buttonTcpConnect_clicked
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   bool checked - is the button checked 
-- NOTES:
-- When the connect button is clicked, create a TCP control channel thread responsible for listening to and
-- keeping track of connected clients. Then, create a TCP service thread for each connected client to
-- service its requests. Clicking the button again closes the TCP control channel socket.
------------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    on_actionJoin_Multicast_triggered
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   bool checked - is the button checked
-- NOTES:
-- When the connect button is clicked, create a TCP control channel thread responsible for listening to and
-- keeping track of connected clients. Then, create a TCP service thread for each connected client to
-- service its requests. Clicking the button again closes the TCP control channel socket.
------------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    HandleNewClient
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   QString ipAddr - IP address of client
--              int socket     - socket of client
-- NOTES:
-- Creates a thread to service a new client and its requests.
------------------------------------------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    PlayMusic
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   N/A
-- NOTES:
-- Tells the playback worker thread to read data off the circular buffer and playback audio.
------------------------------------------------------------------------------------------------------------*/
void MainWindow::PlayMusic() {
    playbackWorker->read_data();
}

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    on_actionAdd_Songs_triggered
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   N/A
-- NOTES:
-- When the action menu to add songs is triggered, get a list of all .wav files in a relative directory
-- and add the item names to the playlist GUI widget.
------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_actionAdd_Songs_triggered() {
    playlist = new Playlist("../Demo");
    playlist->update_list();
    qDebug() << "updated playlist";
    ui->playList->clear();
    ui->playList->addItems(playlist->get_playlist());
    qDebug() << "add playlist items";
}

/*----------------------------------------------------------------------------------------------------------
-- FUNCTION:    on_playList_itemDoubleClicked
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   N/A
-- NOTES:       QListWidgetItem *item - the playlist item that was double clicked
-- Get the name of the playlist item that was clicked and use it to update the "currently playing" label.
------------------------------------------------------------------------------------------------------------*/
void MainWindow::on_playList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->label_6->setText(item->text());
}
