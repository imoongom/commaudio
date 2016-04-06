#include "playback.h"

const int DataSampleRateHz = 44100;

Playback::Playback()
{
    qDebug() << "Heyo";
    wf = new WavFile();
    qByteArray = QByteArray();

    initialize("../Demo/OMFG_-_Hello_(Will__Tim_Remix).wav");
    play();
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
        qBuf.setData(qByteArray);
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
}

void Playback::pause()
{

    m_audioOutput->suspend();
}

void Playback::resume()
{

    m_audioOutput->resume();
}
