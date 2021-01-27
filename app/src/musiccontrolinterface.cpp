#include "musiccontrolinterface.h"
#include "ui_musiccontrolinterface.h"
#include "mainwindow.h"

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

    // Setup Play/Pause buttons
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

    // Setup rewind
    connect(ui->pushButtonRewindRight, &QPushButton::clicked, player, [player]() {
        player->setPosition(player->position() + 10000);
    });
    connect(ui->pushButtonRewindLeft, &QPushButton::clicked, player, [player]() {
        player->setPosition(player->position() - 10000);
    });

    // Setup playlist mode
    connect(ui->pushButtonPlaylistMode, &QPushButton::clicked, playlist, [playlist, this]() {
        switch (playlist->playbackMode()) {
        case QMediaPlaylist::QMediaPlaylist::Sequential:
            playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Random);
            ui->pushButtonPlaylistMode->setIcon(QIcon(":/shuffle.png"));
            break;
        case QMediaPlaylist::QMediaPlaylist::Random:
            playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::CurrentItemInLoop);
            ui->pushButtonPlaylistMode->setIcon(QIcon(":/repeat_one.png"));
            break;
        case QMediaPlaylist::QMediaPlaylist::CurrentItemInLoop:
            playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
            ui->pushButtonPlaylistMode->setIcon(QIcon(":/repeat.png"));
            break;
        case QMediaPlaylist::QMediaPlaylist::Loop:
            playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);
            ui->pushButtonPlaylistMode->setIcon(QIcon(":/sequential.png"));
            break;
        case QMediaPlaylist::QMediaPlaylist::CurrentItemOnce:
            break;
        }
    });
}

MusicControlInterface::~MusicControlInterface()
{
    delete ui;
}
