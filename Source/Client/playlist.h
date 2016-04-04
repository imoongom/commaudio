#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QMap>
#include <QMapIterator>

class Playlist
{
public:
    explicit Playlist(QString filename = "/");
    ~Playlist();
    void update_list();
    QStringList get_playlist();
    QString get_song(QString song);

private:
    QString p_dir;
    QMap<QString, QString> p_files;
    QFileInfo fi;
};

#endif // PLAYLIST_H
