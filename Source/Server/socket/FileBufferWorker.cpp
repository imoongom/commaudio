#include <QApplication>
#include <QDebug>
#include "../Global.h"
#include "FileBufferWorker.h"
#include "circularbuffer.h"

/*-----------------------------------------------------------------------------------------------------
--  SOURCE FILE:    FileBufferWorker.cpp
--  PROGRAM:        COMP 4985 : Comm Audio
--  FUNCTIONS:
--          void FileBufferWorker()
--          void ReadFileAndBuffer()
--          void SendFileToClient(QString songName, SOCKET cSocket) 
--  DATE:           April 9, 2016
--  REVISIONS:      N/A
--  DESIGNERS:      Krystle Bulalakaw
--  PROGRAMMER:     Krystle Bulalakaw
--  NOTES:
--  The worker class for a thread that opens and reads a chunk of a .wav file, writes it
--  to the circular buffer, and emits a signal to nofiy that there is data in the 
--  circular buffer (ready for sending).
-----------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    FileBufferWorker
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     N/A
-- INTERFACE:   N/A
-- NOTES:
-- Constructor for a file buffer worker. Opens a file to be read for playback.
-------------------------------------------------------------------------------------------------------*/
FileBufferWorker::FileBufferWorker()
{
    QString filename = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ServerUDP::InitData file open fail " << filename << ": " << file.errorString();
    }
    qDebug() << "Now playing " << filename;
}

/*-----------------------------------------------------------------------------------------------------
-- FUNCTION:    ReadFileAndBuffer
-- DATE:        April 14, 2016
-- REVISIONS:   N/A
-- DESIGNER:    Krystle Bulalakaw
-- PROGRAMMER:  Krystle Bulalakaw
-- RETURNS:     void
-- INTERFACE:   ReadFileAndBuffer()
-- NOTES:
-- Reads from file and fills the circular buffer until end of file.
-------------------------------------------------------------------------------------------------------*/
void FileBufferWorker::ReadFileAndBuffer(){
    qDebug() << "Running UDPSendWorker";
    QByteArray qByteArray;

    QString filename = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ServerUDP::InitData file open fail " << filename << ": " << file.errorString();
    }
    qDebug() << "Now playing " << filename;
    while (!file.atEnd() && udpConnected) {
        qByteArray = file.read(DATA_BUFSIZE);
        write_buffer(&CBuf, qByteArray.data());
        emit WroteToCBuf();
        Sleep(5); // Prevents speedup and jitter?
    }
}

/* Read from file and fill circular buffer until EOF */
/*
void FileBufferWorker::ReadFileAndBuffer(qint64 pos){
    QByteArray qByteArray;
    if (!file.atEnd() && udpConnected) {
        qDebug() << "Reading at pos " << pos;
        file.seek(pos);
        qByteArray = file.read(DATA_BUFSIZE);
        write_buffer(&CBuf, qByteArray.data());
        emit WroteToCBuf(pos);
        Sleep(5); // Prevents speedup and jitter?
    }
}
*/
