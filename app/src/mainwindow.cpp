#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_playListModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playListModel);
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                           << tr("File Path"));
    ui->playlistView->hideColumn(1);
    ui->playlistView->verticalHeader()->setVisible(false);
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);


    m_player = new QMediaPlayer(this);          // Init player
    m_player->setAudioRole(QAudio::Role::MusicRole);
    m_playlist = new QMediaPlaylist(m_player);  // Init playlist
    m_player->setPlaylist(m_playlist);
    m_player->setVolume(0);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);  // Set circular play mode playlist

    connect(ui->actionAdd_track, &QAction::triggered, this, &MainWindow::on_actionAdd_track_triggered);

    m_musiccontrolinterface = new MusicControlInterface(this);
    this->ui->horizontalLayout_3->addWidget(m_musiccontrolinterface);

    // Volume control
    connect(ui->pushButtonMute, &QPushButton::clicked, m_player, [this](){
        m_muted = !m_muted;
        (m_muted)?m_player->setVolume(0):m_player->setVolume(ui->verticalSliderVolume->value());
    });

    // подключаем кнопки управления к слотам управления
    connect(ui->verticalSliderVolume, &QSlider::valueChanged, m_playlist, [this](int value) {
        if (!m_muted)
            m_player->setVolume(value);
        qDebug() << "=>>>>>>>>>> volume changed: " + QString::number(value);
    });

    // change icon (doesn't work)
    connect(m_player, &QMediaPlayer::currentMediaChanged, this, &MainWindow::metaDataChanged);

    // When you doubleclick on the track in the table set the track in the playlist
    // TODO take tags prom file here
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());        
        qDebug() << "=>>>>>>>>>> double clicke on playListView";
    });

    // if the current track of the index change in the playlist, set the file name in a special label
//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
//        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
//    });

    // Song duration changed
    connect(m_player, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->horizontalSliderSongProgress->setMaximum(duration / 1000);
    });

    // sSong progress changed
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

    const auto deviceInfos = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (const QAudioDeviceInfo &deviceInfo : deviceInfos)
        qDebug() << "Device name: " << deviceInfo.deviceName();

    qDebug() << "supported Codecs";
    for (const auto& it : QAudioDeviceInfo::defaultOutputDevice().supportedCodecs()) {
        qDebug() << it;
    }
    qDebug() << "supported Sample Sizes";
    for (const auto& it : QAudioDeviceInfo::defaultOutputDevice().supportedSampleSizes()) {
        qDebug() << it;
    }
    qDebug() << "supported Sample Types";
    for (const auto& it : QAudioDeviceInfo::defaultOutputDevice().supportedSampleTypes()) {
        qDebug() << it;
    }
    qDebug() << "supported Sample Rates";
    for (const auto& it : QAudioDeviceInfo::defaultOutputDevice().supportedSampleRates()) {
        qDebug() << it;
    }

   metaDataChanged();
}

void MainWindow::metaDataChanged()
{
    qDebug() << "=>>>>>>>>>> metaDataChanged invoked";
    if (m_player->isMetaDataAvailable()) {
        // doesn't work
        qDebug() << "=>>>>>>>>>> there is some metadata...";
//        setTrackInfo(QString("%1 - %2")
//                .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
//                .arg(m_player->metaData(QMediaMetaData::Title).toString()));

        if (ui->coverLabelImage) {
            QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            ui->coverLabelImage->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

//void MainWindow::setTrackInfo(const QString &info)
//{
//    m_trackInfo = info;

//    if (m_statusBar) {
//        m_statusBar->showMessage(m_trackInfo);
//        m_statusLabel->setText(m_statusInfo);
//    } else {
//        if (!m_statusInfo.isEmpty())
//            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
//        else
//            setWindowTitle(m_trackInfo);
//    }
//}

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

MainWindow::~MainWindow()
{
    delete ui;
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void MainWindow::on_actionAdd_track_triggered()
{
    // Using the file selection dialog to make multiple selections of mp3 files
        QStringList files = QFileDialog::getOpenFileNames(this,
                                                          tr("Open files"),
                                                          QString(),
                                                          tr("Audio Files (*.mp3; *.flac)"));



        // Next, set the data names and file paths
        // into the playlist and table displaying the playlist
        foreach (QString filePath, files) {
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QDir(filePath).dirName()));
            items.append(new QStandardItem(filePath));
            m_playListModel->appendRow(items);
            if (isPlaylist(QUrl(filePath)))
                m_playlist->load(QUrl(filePath));
            else {
                m_playlist->addMedia(QUrl(filePath));
            }

        }
    qDebug() << "=>>>>>>>>>> open send";
}
