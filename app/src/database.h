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
#include <QMediaPlaylist>
#include "mytreewidgetitem.h"

#include <map>
//#include "libraryformdialog.h"

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

    bool SavePlaylist(const QString &path, const QString &name)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO Playlist (PName, Path) "
            "VALUES (:PName, :Path)");
        query.bindValue(":PName", name);
        query.bindValue(":Path", path);

        bool exec = query.exec();
        qDebug() << exec;
        return exec;
    }

    std::map<QString, QString> loadPlaylists() {
        std::map<QString, QString> retval;

        QSqlQuery q("SELECT Path FROM Playlist");

        while (q.next())
            qDebug() << q.value(0).toString() << ' ';

        QSqlQuery query("SELECT * FROM Playlist");

        while (query.next()) {
            qDebug() << query.value(0).toString() << "| | " << query.value(1).toString();
            retval.emplace(query.value(0).toString(), query.value(1).toString());
        }
        return retval;

//        QString path = QFileDialog::getOpenFileName(this, tr("Open m3u"), "", tr("(*.m3u)"));
//        if (path != nullptr) {
//            QFile file(path);
//            file.open(QIODevice::ReadOnly | QIODevice::Text);
//            QTextStream in(&file);
//            QString line = in.readLine();
//            int added = m_main->m_db->addToPlaylists(path, m_user);
//            if (added > 0) {
//                MyTreeWidgetItem *my = m_main->m_db->addPlaylist(ui->treeWidget, path, added, m_main);
//                while (!line.isNull()) {
//                    int added_track = m_main->m_db->addToTracks(line);
//                    if (added_track > 0) {
//                        m_main->m_db->addToTrackPlaylists(added_track, added);
//                        m_main->m_db->addTrack(line, my, m_main);
//                    }
//                    line = in.readLine();
//                }
//            }
//        }
    }

    void createDataBase()
    {
        QSqlQuery query;
        //Queue - PK Id int pk auto, FK Path text unique
        // all playlists -> Playlists - Id int pk auto, FK Name text unique, IMAGES BLOB
        // 1 playlist -> SavePlaylist - Id int pk auto, FK Path text

        query.exec("CREATE TABLE IF NOT EXISTS Queue (Id INTEGER PRIMARY KEY AUTOINCREMENT, Path TEXT UNIQUE)");
//        query.exec("CREATE TABLE IF NOT EXISTS AllPlaylists (Name TEXT PRIMARY KEY, Path TEXT UNIQUE, IMAGES BLOB)");
        query.exec("CREATE TABLE IF NOT EXISTS Playlist (PName TEXT PRIMARY KEY, Path TEXT UNIQUE)");
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
