#ifndef MUSICCONTROLINTERFACE_H
#define MUSICCONTROLINTERFACE_H

#include <QWidget>
#include <QMediaPlayer>

class MainWindow;

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
    void isDarkTheme();
    void isLightTheme();

private:
    Ui::MusicControlInterface *ui;
    MainWindow* m_main_window = nullptr;

};

#endif // MUSICCONTROLINTERFACE_H
