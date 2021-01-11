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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PlaylistModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pushButtonPlayPause_clicked();

private:
    Ui::MainWindow *ui;

    PlaylistModel  *m_playListModel = nullptr;   // Data Model for Playlist
    QMediaPlayer        *m_player = nullptr;
    QMediaPlaylist      *m_playlist = nullptr;

private:
    int m_duration;

private:
    void metaDataChanged();
    void setTrackInfo(const QString &info);
    void updateDurationInfo(qint64 currentInfo);
    void on_actionAdd_track_triggered();
};
#endif // MAINWINDOW_H
