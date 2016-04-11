#include "mainwindow.h"
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":qdarkstyle/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(styleSheet);

    w.show();

    return a.exec();
}
