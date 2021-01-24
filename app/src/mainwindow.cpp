#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "musiccontrolinterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupPlaylistModel();

    this->setupPlayer();
    m_playlist = new QMediaPlaylist(m_player);
    m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    m_player->setPlaylist(m_playlist);

    connectActions();

    m_musiccontrolinterface = new MusicControlInterface(this);
    this->ui->horizontalLayout_3->addWidget(m_musiccontrolinterface);
    m_musiccontrolinterface->connectWithMainWinow(m_player, m_playlist);


    // When you doubleclick on the track in the table set the track in the playlist
    // TODO take tags prom file here
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());        
        qDebug() << "=>>>>>>>>>> double click on playListView";
    });


    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
//        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
        ui->playlistView->selectRow(index);
    });



}

void MainWindow::metaDataChanged()
{
    qDebug() << "=>>>>>>>>>> metaDataChanged invoked";


}

MainWindow::~MainWindow()
{
    delete ui;
}
