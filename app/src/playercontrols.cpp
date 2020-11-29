#include "playercontrols.h"
#include "ui_playercontrols.h"

#include <QStyle>

PlayerControls::PlayerControls(QWidget *parent) :
    QWidget(parent),
    isRepeat(false),
    isShuffle(false),
    mode(PMode::NONE),
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
    if (mode == PMode::REPEATALL || mode == PMode::REPEATONE) {
         QMessageBox::warning(this, "Invalid option.", "Please turn off repeating", QMessageBox::Ok);
         return;
    }

    QString pic_on = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle_on.png";
    QString pic_off = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/shuffle.png";

    if (mode == PMode::NONE) {
        ui->shuffle->setIcon(QIcon(pic_on));
        mode = PMode::SHUFFLE;
    } else {
        ui->shuffle->setIcon(QIcon(pic_off));
        mode = PMode::NONE;
    }
    qDebug() << "shuffle Clicked" << isRepeat;
}

void PlayerControls::repeatClicked()
{    
    if (mode == PMode::SHUFFLE) {
        QMessageBox::warning(this, "Invalid option.", "Please turn off shuffle", QMessageBox::Ok);
        return;
    }

    QString pic_repeat = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat.png";
    QString pic_repeat_on = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat_on.png";
    QString pic_repeat_one = QFileSystemModel(this).rootDirectory().absolutePath() + "/uamp/app/resources/repeat_one.png";

    if (mode == PMode::NONE) {
        ui->repeat->setIcon(QIcon(pic_repeat_on));
        qDebug() << "set pic_repeat_on";
        mode = PMode::REPEATALL;
        return;
    } else if (mode == PMode::REPEATALL) {
        ui->repeat->setIcon(QIcon(pic_repeat_one));
        qDebug() << "set pic_repeat_one";
        mode = PMode::REPEATONE;
        return;
    } else if (mode == PMode::REPEATONE) {
        ui->repeat->setIcon(QIcon(pic_repeat));
        qDebug() << "set pic_repeat";
        mode = PMode::NONE;
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

