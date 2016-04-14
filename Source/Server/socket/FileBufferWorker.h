#ifndef FILEBUFFERWORKER_H
#define FILEBUFFERWORKER_H

#include <QObject>
#include <QFile>

class FileBufferWorker : public QObject
{
    Q_OBJECT
public:
    FileBufferWorker();
private slots:
    //void ReadFileAndBuffer(qint64 pos);
    void ReadFileAndBuffer();
signals:
    //void WroteToCBuf(qint64 pos);
    void WroteToCBuf();
private:
    QFile file;
};

#endif // FILEBUFFERWORKER_H
