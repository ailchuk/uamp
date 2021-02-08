#pragma once
#include <QBuffer>
#include <QDir>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTreeWidget>
#include <vector>

class DataBase {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

public:
    explicit DataBase()
    {
        db.setDatabaseName(".data.db");
        db.open();
    }

    ~DataBase()
    {
        db.close();
    }

    void createDataBase()
    {
        QSqlQuery query;
        //Queue - PK Id int pk auto, FK Path text unique
        // all playlists -> Playlists - Id int pk auto, FK Name text unique, IMAGES BLOB
        // 1 playlist -> SavePlaylist - Id int pk auto, FK Path text

        query.exec("CREATE TABLE IF NOT EXISTS Queue (Id INTEGER PRIMARY KEY AUTOINCREMENT, Path TEXT UNIQUE)");
        query.exec("CREATE TABLE IF NOT EXISTS Settings (Id INTEGER PRIMARY KEY AUTOINCREMENT, Volume INT, Sort INT, Type INT)");
        query.prepare("INSERT INTO Settings (Volume) VALUES (:Vol)");
        query.bindValue(":Vol", 0);
        query.exec();
        query.exec("DELETE FROM Settings WHERE Id != 1");

    }

    bool addVolume(int Vol) {
        QSqlQuery query;

        query.prepare("UPDATE Settings SET Volume = :volume WHERE Id = 1;");
        query.bindValue(":volume", Vol);

        return query.exec();
    }

    int GetVolume() {
        QSqlQuery query;

        query.exec("SELECT Volume FROM Settings WHERE Id = 1");
        query.first();

        return query.value(0).toInt();
    }

    bool addToQueue(const QString &path) {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO Queue(Path) "
            "VALUES (:Path)");
        query.bindValue(":Path", path);

        bool exec = query.exec();

        return exec;
    }

    bool deleteTrack(const QString &path) {
        QSqlQuery query;

        query.exec("SELECT Id FROM Queue WHERE Path = '" + path + "';");
        query.first();
        bool exec = query.exec("DELETE FROM Queue WHERE Id = " + query.value(0).toString());

        return exec;
    }

    void loadQueue(QStandardItemModel *m_playListModel, QMediaPlaylist *m_playlist) {
        QSqlQuery query("SELECT Path FROM Queue");

        while (query.next()) {
            QList<QStandardItem *> items;
            QString filePath = query.value(0).toString();

            items.append(new QStandardItem(QDir(filePath).dirName()));
            items.append(new QStandardItem(filePath));
            m_playListModel->appendRow(items);
            m_playlist->addMedia(QUrl(filePath));
        }
    }

    void printQ() {
        QSqlQuery query("SELECT Path FROM Queue");

        while (query.next())
            qDebug() << query.value(0).toString() << ' ';
    }

    void printS() {
       qDebug() << GetVolume();
    }
};
