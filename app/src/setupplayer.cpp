#include "mainwindow.h"
#include "./ui_mainwindow.h"

void MainWindow::setupPlayer() {
    m_player = new QMediaPlayer(this);

    m_player->setAudioRole(QAudio::Role::MusicRole);
    m_player->setVolume(m_db->GetVolume());
    ui->verticalSliderVolume->setValue(m_db->GetVolume());

    // Volume control
    connect(ui->pushButtonMute, &QPushButton::clicked, m_player, [this](){
        m_muted = !m_muted;
        (m_muted) ? m_player->setVolume(0):m_player->setVolume(ui->verticalSliderVolume->value());
    });

    // подключаем кнопки управления к слотам управления
    connect(ui->verticalSliderVolume, &QSlider::valueChanged, m_player, [this](int value) {
        if (!m_muted)
            m_player->setVolume(value);
        qDebug() << "=>>>>>>>>>> volume changed: " + QString::number(value);
    });

    // change icon (doesn't work)
    connect(m_player, &QMediaPlayer::currentMediaChanged, this, &MainWindow::metaDataChanged);

    // Song duration changed
    connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->horizontalSliderSongProgress->setMaximum(duration / 1000);
    });

    // Song progress changed
    connect(m_player, &QMediaPlayer::positionChanged, this, [this](qint64 progress){
        if (!ui->horizontalSliderSongProgress->isSliderDown())
                ui->horizontalSliderSongProgress->setValue(progress / 1000);

            updateDurationInfo(progress / 1000);
    });

    // Drag slider
    ui->horizontalSliderSongProgress->setRange(0, m_player->duration() / 1000);
    connect(ui->horizontalSliderSongProgress, &QSlider::sliderMoved, this, [this](int seconds) {
        m_player->setPosition(seconds * 1000);
        qDebug() << "=>>>>>>>>>> song progress changed: " + QString::number(seconds);
    });

    // Error handling for m_player (redirecting)
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            this, [this](){
        m_player->stop();
        const QString errorString = m_player->errorString();
        qDebug() << (errorString.isEmpty()
                       ? tr("Unknown error #%1").arg(int(m_player->error()))
                       : tr("Error: %1").arg(errorString));
    });

}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) /*+ " / " + totalTime.toString(format)*/;
    }
    ui->recordPositionLabel->setText(tStr);
}
