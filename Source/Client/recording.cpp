#include "recording.h"
#include "global.h"
Recording::Recording()
{
    qDebug() << "wassup";
}

Recording::~Recording()
{

}

void Recording::runthis()
{
    qByteArray = QByteArray();
    qBuf.setBuffer(&qByteArray);
    qBuf.open(QIODevice::ReadWrite);
    m_format = QAudioFormat();

    m_format.setSampleRate(48000); // 48000
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_device = QAudioDeviceInfo::defaultInputDevice();
    if (!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_device.nearestFormat(m_format);
    }
    mic = new micBuffer();
    m_audioInput = new QAudioInput(m_format, mic);
    //m_audioInputDevice = m_audioInput->start(mic);
    m_audioInput->start(mic);
    emit MicDataCapturing(this);

    char *test = (char*)malloc(CIRBUFSIZE);
    return;
    while(m_audioInput->state() == QAudio::ActiveState)
    {
        //qDebug() << "at leat im here";
//      write_buffer(&CBuf, (qBuf.read(CIRBUFSIZE)).data());
        qBuf.read(test, CIRBUFSIZE);
        write_buffer(&CBuf, test);
        //emit writeVoice();
    }
//    m_audioInput->suspend();
}

void Recording::record()
{
    m_audioInput->resume();
    QThread::sleep(5);
    while(!qBuf.buffer().isEmpty())
    {
        qDebug() << "at leat im here";
        write_buffer(&CBuf, (qBuf.read(CIRBUFSIZE)).data());
    }
    qDebug() << "I'm done yo";
}

void Recording::pause()
{
    qDebug() << "woo " << qBuf.size();
    qDebug() << "hmm";
    m_audioInput->suspend();
    qBuf.seek(0);
    m_audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), m_format);
    m_audioOutput->start(&qBuf);
}
