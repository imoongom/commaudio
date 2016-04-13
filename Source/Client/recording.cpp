#include "recording.h"

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
    m_format.setSampleSize(8);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    m_device = QAudioDeviceInfo::defaultInputDevice();
    if (!m_device.isFormatSupported(m_format))
    {
        qWarning()<<"default format not supported try to use nearest";
        m_format = m_device.nearestFormat(m_format);
    }

    m_audioInput = new QAudioInput(m_device, m_format);
    m_audioInput->start(&qBuf);
    while(m_audioInput->state() == QAudio::ActiveState)
    {
        qDebug() << "at leat im here";
        write_buffer(&CBuf, (qBuf.read(CIRBUFSIZE)).data());
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
