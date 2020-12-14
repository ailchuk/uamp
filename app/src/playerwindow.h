#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include "libraryproxymodel.h"
#include <set>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSettings>
#include <QSqlTableModel>
#include <QFileSystemModel>

enum class Mode { NONE, REPEATALL, REPEATONE, SHUFFLE };

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow() override;

private:
    Ui::PlayerWindow *ui;
    QSettings session{"session.ini", QSettings::Format::IniFormat};
    QMediaPlayer *mediaPlayer = nullptr;
    QSqlTableModel *librarySourceModel = nullptr;
    LibraryProxyModel *libraryProxyModel = nullptr;
    QModelIndex srcIndexOfCurrMedia;
    Mode mode;
    QString m_playlist_name;
    QString m_cur_pname;
    std::set<std::string> m_set_playlist;

    void initializeMediaPlayer();
    void initializeLibraryModels();
    void initializeLibraryView();
    void setUpConnections();
    void restoreWindowState();
    void restoreMediaPlayerVolume();
    void restoreLibraryViewState();
    void closeEvent(QCloseEvent *event) override;
    void saveSessionState();
    void saveWindowState();
    void saveMediaPlayerVolume();
    void saveLibraryViewState();
    void insertToTrackTable(const QString &title,
                            const QString &artist,
                            const QString &album,
                            unsigned int trackNum,
                            unsigned int year,
                            const QString &genre,
                            int duration,
                            const QString &location);

    void initializeViewMenu();
    void setShuffleMode();
    void setRepeatMode();

private slots:
    void onAddToLibraryActionTriggered();
    void menuViewActionTriggered(QAction *action);
    void onMediaPlayerStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayOrPauseSignal();
    void setMediaForPlayback(const QModelIndex &index);
    void setNextMediaForPlayback();
    void setPreviousMediaForPlayback();
    void updateCurrTrackLabel();
    void updateLibraryViewSelection();
    void updateTimePassed(int position);
    void customContextMenu(const QPoint &point);
    void deleteFromLibrary();
    void on_pushButton_clicked();
};

#endif // PLAYERWINDOW_H
