#include "playback.h"


Playback::Playback()
{
    qDebug() << "hi.";
}

Playback::~Playback()
{
    qDebug() << "Good bye ya";
}

void Playback::runthis()
{
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    wf = new WavFile();
    wf->open("../Demo/OMFG_-_Hello_(Will__Tim_Remix).wav");

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, wf->fileFormat());
    m_audioOutput->start(&qBuf);
    read_data();
}

// Testing
void Playback::write_data()
{
    char *write = (char*)malloc(BUFSIZE);

    for (int i = 0; i < CIRBUFMAX; i++)
    {
        wf->read(write, BUFSIZE);
        write_buffer(bufs, write);
    }
}


void Playback::read_data()
{
    char *readbuf = (char*)malloc(CIRBUFSIZE);
    char *testing = (char*)malloc(CIRBUFSIZE);
    QByteArray qba;
    for (int i = 0; i < 100; i++)
    {
        write_buffer(&CBuf, (wf->read(CIRBUFSIZE)).data());
    }
    for (int i = 0; i < 100; i++)
    {
        if(CBuf._count != 0)
        {
            read_buffer(&CBuf, testing);
            qba = QByteArray(testing, CIRBUFSIZE);
            qByteArray.append(qba);
        }
    }

//    for(int i = 0; i < 200; i++)
//    {
//        qByteArray.append(wf->read(CIRBUFSIZE));
//    }
//    for(int i = 0; i < 200; i++)
//    {
//        qByteArray.append(wf->read(CIRBUFSIZE));
//    }
}
