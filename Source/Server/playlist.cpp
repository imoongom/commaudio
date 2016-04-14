/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: playlist.cpp
--
-- PROGRAM: CommAudio
--
-- FUNCTIONS:
    explicit Playlist(QString filename = "/");
    ~Playlist();
    void update_list();
    QStringList get_playlist();
    QString get_song(QString song);
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- PROGRAMMER: Eunwon Moon, Oscar Kwan, Gabriel Lee, Krystle Bulalakaw
--
-- NOTES:
-- The class to get a list of files from the given directory.
----------------------------------------------------------------------------------------------------------------------*/
#include "playlist.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Playlist
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: n/a
--
-- NOTES:
-- Constructor
----------------------------------------------------------------------------------------------------------------------*/
Playlist::Playlist(QString filename)
{
    qDebug() << "Hiya";
    p_dir = filename;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~Playlist
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: n/a
--
-- NOTES:
-- Destructor
----------------------------------------------------------------------------------------------------------------------*/
Playlist::~Playlist()
{

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: update_list
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: void
--
-- NOTES:
-- The function to update the list of files in the directory.
----------------------------------------------------------------------------------------------------------------------*/
void Playlist::update_list()
{
    QDirIterator it(p_dir, QStringList() << "*.wav", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        fi.setFile(it.next());
        p_files.insert(fi.fileName(), fi.absoluteFilePath());
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_playlist
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: QStringList
--
-- NOTES:
-- Getter function to return the list of files in a QStringList.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: get_song
--
-- DATE: April 14, 2016
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- RETURNS: QString
--
-- NOTES:
-- Getter function to return absolute address of the given song name.
----------------------------------------------------------------------------------------------------------------------*/
QString Playlist::get_song(QString song)
{
    if(p_files.contains(song))
        return p_files.value(song);
    return "";
}
