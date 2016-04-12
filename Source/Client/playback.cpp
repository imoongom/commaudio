#include "playback.h"

const int DataSampleRateHz = 44100;

Playback::Playback()
{
    qDebug() << "Heyo";
    wf = new WavFile();
    qByteArray = QByteArray(); // QByteArray(1024, NULL);

    initialize("../Demo/OMFG_-_Hello_(Will__Tim_Remix).wav");
    play();
}

Playback::Playback(RingBuffer *buf)
{
    char *cbuf = (char*)malloc(BUFSIZE);
    char *tmp = (char*)malloc(BUFSIZE);
    char *tmp2 = (char*)malloc(BUFSIZE*5);

    wf = new WavFile();

    if(wf->open("../Demo/OMFG_-_Hello_(Will__Tim_Remix).wav"))
    {
        /* This part seems to be broken.
        while(wf->read(tmp, BUFSIZE))
        {
            qDebug() << "in read";
            if (!buf->push(tmp))
                qDebug() << "oh dang";
            memset(tmp, '\0', BUFSIZE);
        }
        */
        wf->read(tmp, BUFSIZE);
    }
    qDebug() << "done read";

    qByteArray = QByteArray(tmp, BUFSIZE);
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);

    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_format = wf->fileFormat();
    m_audioOutput = new QAudioOutput(m_device, m_format);
    /* Figure out how to change, replace or append data to the buffer
    if (!buf->pop(cbuf))
        qDebug() << "hot diggity";
        */
    qDebug() << "bytearray" << qByteArray.size() << qByteArray.count() << qByteArray.isEmpty() << qByteArray.data();
    m_audioOutput->start(&qBuf);
    qDebug() << "qbuf" << qBuf.size() << qBuf.bytesAvailable();

    // How to continuously play? Check if it's done? Continuously add data?
}

Playback::~Playback()
{
    qDebug() << "Good bye ya";
}

bool Playback::initialize(const QString &fileName)
{
    if(wf->open(fileName))
    {
        m_format = wf->fileFormat();
        qByteArray = wf->readAll();
        qBuf.setBuffer(&qByteArray);
        qBuf.open(QIODevice::ReadWrite);
        return true;
    }
    return false;
}

void Playback::play()
{
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = new QAudioOutput(m_device, m_format);
    m_audioOutput->start(&qBuf);
    qDebug() << qBuf.size() << qBuf.bytesAvailable();
}

void Playback::pause()
{
    m_audioOutput->suspend();
}

void Playback::resume()
{

    m_audioOutput->resume();
}
