#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QDir>
//#include <QTimer>
//#include <QMimeData>
//#include <QDragEnterEvent>

#include "Playlist.h"
#include "Track.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UAmp; }
QT_END_NAMESPACE


class UAmp : public QMainWindow
{
    Q_OBJECT

    Ui::UAmp *ui;
    QStandardItemModel  *m_playListModel;   // Data Model for Playlist
    QMediaPlaylist      *m_playlist;
    QMediaPlayer        *m_player;
//    void dropEvent(QDropEvent *e);
//    void dragEnterEvent(QDragEnterEvent *e);
public:
    UAmp(QWidget *parent = nullptr);
    ~UAmp();

    void addNewSong(const QString &path);

private slots:
    void on_b_add_clicked();
};
