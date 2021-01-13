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

    // подключаем кнопки управления к слотам управления
    connect(ui->verticalSliderVolume, &QSlider::valueChanged, m_playlist, [this](int value) {
        m_player->setVolume(value);
        qDebug() << "=>>>>>>>>>> volume changed: " + QString::number(value);
    });
    connect(ui->pushButtonPrevious, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);
    connect(ui->pushButtonNext, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(ui->pushButtonStop, &QPushButton::clicked, m_player, &QMediaPlayer::stop);

    // change icon (doesn't work)
    connect(m_player, &QMediaPlayer::currentMediaChanged, this, &MainWindow::metaDataChanged);

    // When you doubleclick on the track in the table set the track in the playlist
    // TODO take tags prom file here
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());        
        qDebug() << "=>>>>>>>>>> double clicke on playListView";

    });

    // if the current track of the index change in the playlist, set the file name in a special label
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
    });

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
        qDebug() << "=>>>>>>>>>> volume progress changed: " + QString::number(seconds);
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

    connect(m_player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::PlayingState) {
            ui->pushButtonPlayPause->setIcon(QIcon(":/pause.png"));
        }
        else {
            ui->pushButtonPlayPause->setIcon(QIcon(":/play.png"));
        }
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


void MainWindow::on_pushButtonPlayPause_clicked() {
    if (m_player->state() == QMediaPlayer::State::PausedState
            || m_player->state() == QMediaPlayer::State::StoppedState) {
        m_player->play();
    }
    else {
        m_player->pause();
    }
}

void MainWindow::metaDataChanged()
{
    qDebug() << "=>>>>>>>>>> metaDataChanged invoked";
    if (m_player->isMetaDataAvailable()) {
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
                //m_playlist->addMedia(QUrl(filePath));



                QFile audio_file(filePath);
                audio_file.open(QIODevice::ReadOnly);
                //audio_file.seek(32); // skip wav header

                // initialize parameters
                int sampleRate = 96000;   // sample rate

                // --- transfer QVector data to QByteBuffer
                QByteArray* byteBuffer = new QByteArray(audio_file.readAll());  // create a new instance of QByteArray class (in the heap, dynamically arranged in memory), and set its pointer to byteBuffer
                qDebug() << byteBuffer->size();
                audio_file.close();

                // create and setup a QAudioFormat object
                QAudioFormat audioFormat;
                audioFormat.setSampleRate(sampleRate);
                audioFormat.setChannelCount(2);
                audioFormat.setSampleSize(32);   // set the sample size in bits. We set it to 32 bis, because we set SampleType to float (one float has 4 bytes ==> 32 bits)
                audioFormat.setCodec("audio/pcm");
                audioFormat.setByteOrder(QAudioFormat::LittleEndian);
                audioFormat.setSampleType(QAudioFormat::Float);   // use Float, to have a better resolution than SignedInt or UnSignedInt

                QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());




                // create a QAudioDeviceInfo object, to make sure that our audioFormat is supported by the device
                QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
                if(!deviceInfo.isFormatSupported(audioFormat))
                {
                    audioFormat = deviceInfo.nearestFormat(audioFormat);
                    qWarning() << "Raw audio format not supported by backend, cannot play audio.";
                }

                // Make a QBuffer with our QByteArray
                QBuffer* input = new QBuffer(byteBuffer);
                input->open(QIODevice::ReadOnly);   // set the QIODevice to read-only
                qDebug() << input->size();

                // Create an audio output with our QAudioFormat
                QAudioOutput* audio = new QAudioOutput(audioFormat, this);

                // connect up signal stateChanged to a lambda to get feedback
                connect(audio, &QAudioOutput::stateChanged, [audio, input](QAudio::State newState)
                {
                    if (newState == QAudio::IdleState)   // finished playing (i.e., no more data)
                    {
                        qDebug() << "finished playing sound";
                        delete audio;
                        delete input;
                        //delete byteBuffer;  // I tried to delete byteBuffer pointer (because it may leak memories), but got compiler error. I need to figure this out later.
                    }
                    // should also handle more states, e.g., errors. I need to figure out on how to do this later.
                });

                // start the audio (i.e., play sound from the QAudioOutput object that we just created)
                audio->start(input);

            }

        }
    qDebug() << "=>>>>>>>>>> open send";
}
