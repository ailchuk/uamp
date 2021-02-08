#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QThread>
#include <QFuture>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QDir>

#include "database.h"

class MusicControlInterface;
class libraryformdialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadSettings(void);

    DataBase *m_db; // database
private slots:

    void on_actionShow_tags_triggered();

    void on_actionSave_playlist_triggered();

    void on_actionLight_triggered();

    void on_actionDark_triggered();

public:
    Ui::MainWindow *ui;

    QStandardItemModel      *m_playListModel = nullptr;   // Data Model for Playlist
    QMediaPlayer            *m_player = nullptr;
    QMediaPlaylist          *m_playlist = nullptr;
    MusicControlInterface   *m_musiccontrolinterface = nullptr;
    libraryformdialog       *m_libraryform = nullptr;

private:
    int m_duration;
    bool m_muted = false;

private:
    void metaDataChanged();
    void setTrackInfo(const QString &info);
    void updateDurationInfo(qint64 currentInfo);
    void on_actionAdd_track_triggered();
    void setupPlayer();
    void setupPlaylistModel();
    void connectActions();
    void saveSessionToDb();

};
#endif // MAINWINDOW_H
