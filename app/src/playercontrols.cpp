#include "playercontrols.h"
#include "ui_playercontrols.h"

#include <QStyle>

PlayerControls::PlayerControls(QWidget *parent) :
    QWidget(parent),
    isRepeat(false),
    isShuffle(false),
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
//    ui->shuffle->setIcon(QIcon(QFileSystemModel(this).rootDirectory().absolutePath() + "/app/resources/shuffle.png")); // for macos
//    ui->repeat->setIcon(QIcon(QFileSystemModel(this).rootDirectory().absolutePath() + "/app/resources/repeat.png")); // for macos
    ui->shuffle->setIcon(QIcon(QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle.png")); // for win
    ui->repeat->setIcon(QIcon(QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat.png")); // for win

    ui->shuffle->icon().setThemeName(QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle.png");
    ui->repeat->icon().setThemeName(QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat.png");
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
    connect(ui->repeat, &QAbstractButton::clicked, this, &PlayerControls::repeatClicked);
    connect(ui->repeat, &QAbstractButton::clicked, this, &PlayerControls::setRepeatMode);
    connect(ui->shuffle, &QAbstractButton::clicked, this, &PlayerControls::shuffleClicked);
    connect(ui->shuffle, &QAbstractButton::clicked, this, &PlayerControls::setShuffleMode);
}

void PlayerControls::shuffleClicked()
{
    isShuffle = isShuffle ? false : true;
    if (isRepeat) {
         QMessageBox::warning(this, "Invalid option.", "Please turn off repeating", QMessageBox::Ok);
         return;
    }
    QString pic_on = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle_on.png";
    QString pic_off = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle.png";

    if (ui->shuffle->icon().themeName() == pic_on) {
        ui->shuffle->setIcon(QIcon(pic_off));
        ui->shuffle->icon().setThemeName(pic_off);
    } else {
        ui->shuffle->setIcon(QIcon(pic_on));
        ui->shuffle->icon().setThemeName(pic_on);
    }
    qDebug() << "shuffle Clicked" << isRepeat;
}

void PlayerControls::repeatClicked()
{
    if (isShuffle) {
        QMessageBox::warning(this, "Invalid option.", "Please turn off shuffle", QMessageBox::Ok);
        return;
    }

    QString pic_repeat = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat.png";
    QString pic_repeat_on = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat_on.png";
    QString pic_repeat_one = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat_one.png";
    QString themeName = ui->repeat->icon().themeName();

    if (themeName == pic_repeat) {
        ui->repeat->setIcon(QIcon(pic_repeat_on));
        ui->repeat->icon().setThemeName(pic_repeat_on);
        qDebug() << "set pic_repeat_on";
        isRepeat = true;
        return;
    } else if (themeName == pic_repeat_on) {
        ui->repeat->setIcon(QIcon(pic_repeat_one));
        ui->repeat->icon().setThemeName(pic_repeat_one);
        qDebug() << "set pic_repeat_one";
        isRepeat = true;
        return;
    } else if (themeName == pic_repeat_one) {
        ui->repeat->setIcon(QIcon(pic_repeat));
        ui->repeat->icon().setThemeName(pic_repeat);
        qDebug() << "set pic_repeat";
        isRepeat = false;
        return;
    }
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

