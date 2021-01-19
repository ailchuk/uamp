#ifndef MUSICCONTROLINTERFACE_H
#define MUSICCONTROLINTERFACE_H

#include <QWidget>
#include "mainwindow.h"
#include <QMediaPlayer>

namespace Ui {
class MusicControlInterface;
}

class MusicControlInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MusicControlInterface(QWidget *parent = nullptr);
    ~MusicControlInterface();

    void connectWithMainWinow(QMediaPlayer *player, QMediaPlaylist *playlist);

private:
    Ui::MusicControlInterface *ui;
    MainWindow* m_main_window = nullptr;

};

#endif // MUSICCONTROLINTERFACE_H
