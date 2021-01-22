#pragma once
#include <QBuffer>
#include <QDir>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTreeWidget>

#include "generalwindow.h"
#include "mytreewidgetitem.h"

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

        query.exec("CREATE TABLE IF NOT EXISTS Tracks (NumbTrack INTEGER PRIMARY KEY AUTOINCREMENT, Path TEXT UNIQUE, Stars INT, Rating INT)");
        query.exec("CREATE TABLE IF NOT EXISTS Playlists (NumbPlaylist INTEGER PRIMARY KEY AUTOINCREMENT, NumbUser INT, Name TEXT UNIQUE, IMAGES BLOB)");
        query.exec("CREATE TABLE IF NOT EXISTS TrackPlaylists (NumbTrack INT, NumbPlaylist INT, PRIMARY KEY (NumbTrack, NumbPlaylist))");
        query.exec("CREATE TABLE IF NOT EXISTS Users (NumbUser INTEGER PRIMARY KEY AUTOINCREMENT, User TEXT UNIQUE, Pass TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS SavePlaylist (NumbTrack INTEGER PRIMARY KEY AUTOINCREMENT, Path TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS Save (Numb INTEGER PRIMARY KEY AUTOINCREMENT, Id INT, Pos INT, Volume INT, Sort INT, Type INT)");

        query.prepare(
            "INSERT INTO Save (Volume) "
            "VALUES (:Vol)");
        query.bindValue(":Vol", 70);
        query.exec();
        query.exec("DELETE FROM Save WHERE Numb != 1");
    }

    int GetId()
    {
        QSqlQuery query;

        query.exec("SELECT Id FROM Save WHERE Numb = 1");
        query.first();

        return query.value(0).toInt();
    }

    QString GetTrack()
    {
        QSqlQuery query;

        query.exec("SELECT path FROM SavePlaylist WHERE NumbTrack = " + QString::number(GetId()));
        query.first();

        if (query.size() > 0)
            return query.value(0).toString();

        return nullptr;
    }

    void addIntoSavePlaylist(const QString &path)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO SavePlaylist (Path) "
            "VALUES (:Path)");
        query.bindValue(":Path", path);
        query.exec();
    }

    QList<QString> GetSavePlaylist()
    {
        QSqlQuery query;
        QList<QString> list;

        query.exec("SELECT Path FROM SavePlaylist");

        if (query.first())
            list.push_back(query.value(0).toString());

        while (query.next())
            list.push_back(query.value(0).toString());

        return list;
    }

    void deleteSavePlaylist()
    {
        QSqlQuery query;

        query.exec("DELETE FROM SavePlaylist;");
    }

    void addVolumeAndPos(int Vol, int Pos)
    {
        QSqlQuery query;

        query.prepare("UPDATE Save SET Volume = :Vol, Pos = :pos WHERE Numb = 1;");
        query.bindValue(":Vol", Vol);
        query.bindValue(":pos", Pos);
        query.exec();
    }

    void addId(int id)
    {
        QSqlQuery query;

        query.prepare("UPDATE Save SET Id = :Id WHERE Numb = 1;");
        query.bindValue(":Id", id);
        query.exec();
    }

    void addTypeSort(int Type, int Sort)
    {
        QSqlQuery query;

        query.prepare("UPDATE Save SET Sort = :Sort, Type = :Type WHERE Numb = 1;");
        query.bindValue(":Sort", Sort);
        query.bindValue(":Type", Type);
        query.exec();
    }

    int GetVolume()
    {
        QSqlQuery query;

        query.exec("SELECT Volume FROM Save WHERE Numb = 1");
        query.first();

        return query.value(0).toInt();
    }

    int GetPos()
    {
        QSqlQuery query;

        query.exec("SELECT Pos FROM Save WHERE Numb = 1");
        query.first();

        return query.value(0).toInt();
    }

    int GetSort()
    {
        QSqlQuery query;

        query.exec("SELECT Sort FROM Save WHERE Numb = 1");
        query.first();

        return query.value(0).toInt();
    }

    int GetType()
    {
        QSqlQuery query;

        query.exec("SELECT Type FROM Save WHERE Numb = 1");
        query.first();

        return query.value(0).toInt();
    }

    bool SignUp(const QString &user, const QString &pass)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO Users (User, Pass) "
            "VALUES (:User, :Pass)");
        query.bindValue(":User", user);
        query.bindValue(":Pass", pass);

        return query.exec();
    }

    bool SignIn(const QString &user, const QString &pass)
    {
        QSqlQuery query;

        query.exec("SELECT User FROM Users WHER Pass = '" + pass + "';");
        query.first();

        return query.value(0).toString() == user ? true : false;
    }

    int GetUserId(const QString &user)
    {
        QSqlQuery query;

        query.exec("SELECT NumbUser FROM Users WHERE User = '" + user + "';");
        query.first();

        return query.value(0).toInt();
    }

    void selectPlaylistFromDataBase(QTreeWidget *treeWidget, const QString &user, generalWindow *m_main)
    {
        QSqlQuery query;

        query.exec("SELECT Name, NumbPlaylist FROM Playlists WHERE NumbUser = " + QString::number(GetUserId(user)));

        while (query.next())
            addPlaylist(treeWidget, query.value(0).toString(), query.value(1).toInt(), m_main);
    }

    int NumbPlaylist(const QString &name)
    {
        QSqlQuery query;

        query.exec("SELECT NumbPlaylist FROM Playlists WHERE Name = '" + name + "';");
        query.first();

        return query.value(0).toInt();
    }

    int addToPlaylists(const QString &name, const QString &user)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO Playlists (Name, NumbUser) "
            "VALUES (:Name, :NumbUser)");
        query.bindValue(":Name", name);
        query.bindValue(":NumbUser", GetUserId(user));

        bool exec = query.exec();

        if (exec == 0)
            return -1;

        return NumbPlaylist(name);
    }

    int addToTracks(const QString &path)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO Tracks(Path, Stars, Rating) "
            "VALUES (:Path, :Stars, :Rating)");
        query.bindValue(":Path", path);
        query.bindValue(":Stars", 0);
        query.bindValue(":Rating", 0);
        query.exec();
        query.exec("SELECT NumbTrack FROM Tracks WHERE Path = '" + path + "';");
        query.first();

        return query.value(0).toInt();
    }

    void deleteTrack(const QString &path)
    {
        QSqlQuery query;

        query.exec("SELECT NumbTrack FROM Tracks WHERE Path = '" + path + "';");
        query.first();
        query.exec("DELETE FROM TrackPlaylists WHERE NumbTrack = " + query.value(0).toString());
    }

    bool addToTrackPlaylists(int track, int playlist)
    {
        QSqlQuery query;

        query.prepare(
            "INSERT INTO TrackPlaylists(NumbTrack, NumbPlaylist) "
            "VALUES (:NumbTrack, :NumbPlaylist)");
        query.bindValue(":NumbTrack", track);
        query.bindValue(":NumbPlaylist", playlist);

        return query.exec();
    }

    void deletePlaylists(const QString &name)
    {
        QSqlQuery query;

        query.exec("SELECT NumbPlaylist FROM Playlists WHERE Name = '" + name + "';");
        query.first();

        QString string = query.value(0).toString();

        query.exec("DELETE from TrackPlaylists WHERE NumbPlaylist = " + string);
        query.exec("DELETE FROM Playlists WHERE NumbPlaylist = " + string);
    }

    bool renamePlaylist(const QString &string, const QString &newName)
    {
        QSqlQuery query;

        return query.exec("UPDATE Playlists SET Name = '" + newName + "' WHERE Name ='" + string + "';");
    }

    void insertImage(const QString &name, const QString &filename)
    {
        QFile file(filename);

        file.open(QIODevice::ReadOnly);

        QByteArray ba = file.readAll();

        QSqlQuery query;
        query.prepare("UPDATE Playlists SET IMAGES = :image WHERE Name = :name;");
        query.bindValue(":image", ba, QSql::InOut | QSql::Binary);
        query.bindValue(":name", name);
        query.exec();
        file.close();
    }

    QByteArray GetImage(const QString &name)
    {
        QSqlQuery query;

        query.exec("SELECT IMAGES FROM Playlists WHERE Name = '" + name + "';");
        query.first();

        return query.value(0).toByteArray();
    }

    void saveImage(const QString &filename)
    {
        QByteArray mass = GetImage(filename);
        QPixmap pixmap = QPixmap();

        pixmap.loadFromData(mass);

        QFile file(filename + ".png");
        QBuffer buffer(&mass);

        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&file, "PNG");
    }

    void deleteImage(const QString &name)
    {
        QSqlQuery query;

        query.exec("UPDATE Playlists SET IMAGES = '' WHERE Name = '" + name + "';");

        QFile::remove(name + ".png");
    }
};
