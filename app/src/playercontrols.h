#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QSettings>
#include <QWidget>


namespace Ui {
class PlayerControls;
}

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);
    ~PlayerControls();
    const int DEFAULT_VOLUME = 100;

private:
    Ui::PlayerControls *ui;
    QSettings session{"session.ini", QSettings::Format::IniFormat};
    void setIcons();
    void restoreVolumeSliderState();
    void setConnections();

signals:
    void nextClicked();
    void pauseClicked();
    void rewindClicked();
    void fast_forwardClicked();
    void playOrPauseClicked();
    void prevClicked();
    void progressSliderMoved(int value);
    void volumeChanged(int value);

public slots:
    void updatePlaybackState(QMediaPlayer::State mediaState);
    void setupProgressSlider(int mediaDurationInMillisec);
    void updateProgressSlider(int position);
};

#endif // PLAYERCONTROLS_H
