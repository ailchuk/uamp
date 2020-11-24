#include "playerwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

bool initializeTrackTable()
{
/* Creates (if it does not already exist) the media database table "Track", which stores info
 * about each track/song added to this Media Player program's library.
 * Returns false only if the query could not be executed, not whether or not the table exists.
*/
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS Track ("
                    "id          INTEGER PRIMARY KEY,"
                    "title       VARCHAR(255),"
                    "artist      VARCHAR(255),"
                    "album       VARCHAR(255),"
                    "track_num   INTEGER,"
                    "year        INTEGER,"
                    "genre       VARCHAR(255),"
                    "duration    INTEGER,"
                    "location    TEXT UNIQUE NOT NULL)"))
    {
        return false;
    }

    return true;
}


bool initializeMediaDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/mediadb.sqlite");
    if (!db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open media database"),
            QObject::tr("Unable to establish a connection to the media database.\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    if (!initializeTrackTable())
    {
        return false;
    }

    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!initializeMediaDb())
    {
        return EXIT_FAILURE;
    }

    PlayerWindow w;
    w.show();

    return a.exec();
}
