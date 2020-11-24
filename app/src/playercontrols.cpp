#include "playercontrols.h"
#include "ui_playercontrols.h"

#include <QStyle>

PlayerControls::PlayerControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControls)
{
    ui->setupUi(this);
    setIcons();
    restoreVolumeSliderState();
    setConnections();
}


PlayerControls::~PlayerControls()
{
    delete ui;
}


void PlayerControls::setIcons()
{
    ui->playOrPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->rewind->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->fast_forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->volumeLabel->setPixmap(style()->standardIcon(QStyle::SP_MediaVolume).pixmap(QSize(25,25)));
}


void PlayerControls::restoreVolumeSliderState()
{
    int volume = session.value("MediaPlayer/volume", DEFAULT_VOLUME).toInt();
    ui->volumeSlider->setValue(volume);
}


void PlayerControls::setConnections()
{
    connect(ui->playOrPauseButton, &QAbstractButton::clicked, this, &PlayerControls::playOrPauseClicked);
    connect(ui->prevButton, &QAbstractButton::clicked, this, &PlayerControls::prevClicked);
    connect(ui->nextButton, &QAbstractButton::clicked, this, &PlayerControls::nextClicked);
    connect(ui->rewind, &QAbstractButton::clicked, this, &PlayerControls::rewindClicked);
    connect(ui->fast_forward, &QAbstractButton::clicked, this, &PlayerControls::fast_forwardClicked);
    connect(ui->volumeSlider, &QAbstractSlider::valueChanged, this, &PlayerControls::volumeChanged);
    connect(ui->progressSlider, &PlayerControlsSlider::sliderMoved, this, &PlayerControls::progressSliderMoved);
}


void PlayerControls::updateProgressSlider(int position)
{
    if (!ui->progressSlider->isSliderDown())
    {
        ui->progressSlider->setValue(position);
    }

    /* If you allowed the progress slider to be updated while it was being held down, then the
       song would keep trying to reset to the point where the slider is being held down,
       therefore we need to check that the slider is not being held down.
    */
}


void PlayerControls::setupProgressSlider(int mediaDurationInMillisec)
{
    ui->progressSlider->setMaximum(mediaDurationInMillisec);
}


void PlayerControls::updatePlaybackState(QMediaPlayer::State mediaState)
{
    if (mediaState == QMediaPlayer::State::PlayingState)
    {
        ui->playOrPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else
    {
        ui->playOrPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

