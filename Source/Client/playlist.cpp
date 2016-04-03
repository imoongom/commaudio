#include "playlist.h"

Playlist::Playlist(QString filename)
{
    qDebug() << "Hiya";
    p_dir = filename;
}

Playlist::~Playlist()
{

}

void Playlist::update_list()
{
    QDirIterator it(p_dir, QStringList() << "*.wav", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        fi.setFile(it.next());
        p_files.insert(fi.fileName(), fi.absoluteFilePath());
    }
}

QStringList Playlist::get_playlist()
{
    QStringList list;
    QMapIterator<QString, QString> i(p_files);
    while(i.hasNext())
    {
        list << i.next().key();
    }

    return list;
}
