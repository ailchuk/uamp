#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "musiccontrolinterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_db = new DataBase();
    m_db->createDataBase();

    setupPlaylistModel();

    this->setupPlayer();
    m_playlist = new QMediaPlaylist(m_player);
    m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    m_player->setPlaylist(m_playlist);

    connectActions();

    m_musiccontrolinterface = new MusicControlInterface(this);
    this->ui->verticalLayout_2->addWidget(m_musiccontrolinterface);
    m_musiccontrolinterface->connectWithMainWinow(m_player, m_playlist);

    m_db->loadQueue(m_playListModel, m_playlist); // loads last save queue

    QString savedsong = m_db->GetSaveTrack();
    if (savedsong.size() > 0) {
        int i = 0;
        auto model = ui->playlistView->selectionModel()->selectedIndexes();
        for (auto row : model) {
            if (savedsong == m_playListModel->itemFromIndex(row)->text()) {
                ui->playlistView->selectRow(i / 2);
                qDebug() << "QQQQQ";
                break;
            }
            i++;
        }
        qDebug() << "SQ";
//        m_playlist->addMedia(QMediaContent(QUrl::fromLocalFile(m_db->GetSaveTrack())));
//        m_player->setPosition(m_db->GetPos() / 1000);

//        ui->horizontalSliderSongProgress->setValue(m_db->GetPos() / 1000);

//        updateDurationInfo(m_db->GetPos() / 1000);
    }
    // When you doubleclick on the track in the table set the track in the playlist
    // TODO take tags prom file here
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());        
        qDebug() << "=>>>>>>>>>> double click on playListView";
    });

    // Keep current song selected in the listview
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->playlistView->selectRow(index);
    });

}

void MainWindow::metaDataChanged()
{
    qDebug() << "=>>>>>>>>>> metaDataChanged invoked";

}

void MainWindow::saveSessionToDb() {
    m_db->addVolumeAndPos(m_player->volume(), m_player->position());
    m_db->addSaveTrack(m_playlist->currentMedia().canonicalUrl().toString());
}

MainWindow::~MainWindow()
{
    saveSessionToDb();
    delete ui;
    m_db->printS();
    delete m_db;
}
