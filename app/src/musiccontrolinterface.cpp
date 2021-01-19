#include "musiccontrolinterface.h"
#include "ui_musiccontrolinterface.h"

MusicControlInterface::MusicControlInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicControlInterface)
{
    ui->setupUi(this);
    m_main_window = qobject_cast<MainWindow*>(parent);
}

void MusicControlInterface::connectWithMainWinow(QMediaPlayer *player, QMediaPlaylist *playlist) {
    // Music control interface
    connect(ui->pushButtonPrevious, &QPushButton::clicked, playlist, &QMediaPlaylist::previous);
    connect(ui->pushButtonNext, &QPushButton::clicked, playlist, &QMediaPlaylist::next);
    connect(ui->pushButtonStop, &QPushButton::clicked, player, &QMediaPlayer::stop);

    connect(ui->pushButtonPlayPause, &QPushButton::clicked, this, [player]() {
        if (player->state() == QMediaPlayer::State::PausedState
            || player->state() == QMediaPlayer::State::StoppedState) {
            player->play();
        }
        else {
            player->pause();
        }
    });

    connect(player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::PlayingState) {
            ui->pushButtonPlayPause->setIcon(QIcon(":/pause.png"));
        }
        else {
            ui->pushButtonPlayPause->setIcon(QIcon(":/play.png"));
        }
    });
}

MusicControlInterface::~MusicControlInterface()
{
    delete ui;
}
