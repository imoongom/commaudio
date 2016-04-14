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
    void ReadFileAndBuffer(qint64 pos);
signals:
    void WroteToCBuf(qint64 pos);
private:
    QFile file;
};

#endif // FILEBUFFERWORKER_H
