#include <QApplication>
#include <QDebug>
#include "../Global.h"
#include "FileBufferWorker.h"
#include "../Client/socket/circularbuffer.h"

FileBufferWorker::FileBufferWorker()
{
    QString filename = "../Demo/Party_In_The_USA-Miley_Cyrus.wav";
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "ServerUDP::InitData file open fail " << filename << ": " << file.errorString();
    }
    qDebug() << "Now playing " << filename;
}

/* Read from file and fill circular buffer until EOF */
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
