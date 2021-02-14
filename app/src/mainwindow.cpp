#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "musiccontrolinterface.h"
#include "libraryformdialog.h"
#include "dtagmusic.h"

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

    this->ui->verticalLayout_3->setAlignment(Qt::AlignCenter);
    this->ui->verticalLayout->setAlignment(Qt::AlignLeft);
    m_musiccontrolinterface = new MusicControlInterface(this);
    this->ui->verticalLayout_3->addWidget(m_musiccontrolinterface);
    m_musiccontrolinterface->connectWithMainWinow(m_player, m_playlist);

    m_libraryform = new libraryformdialog(this);
    m_libraryform->hide();
    m_libraryform->setDataBase(*m_db);

    m_dtag = new DTagMusic(this);
    m_dtag->hide();

    m_db->loadQueue(m_playListModel, m_playlist); // loads last save queue

    // When you doubleclick on the track in the table set the track in the playlist
    // TODO take tags from file here
    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());
        QString path = m_playlist->currentMedia().request().url().toString();
        m_dtag->setTagWindow(path);
        ui->coverLabelImage->setPixmap(QPixmap::fromImage(m_dtag->m_coverImage));
        qDebug() << "=>>>>>>>>>> double click on playListView";
    });

    // Keep current song selected in the listview
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
        ui->playlistView->selectRow(index);
    });

    on_actionLight_triggered(); // set as default light theme;
}

void MainWindow::metaDataChanged()
{
    qDebug() << "=>>>>>>>>>> metaDataChanged invoked";

}

void MainWindow::saveSessionToDb() {
    m_db->addVolume(m_player->volume());
}

MainWindow::~MainWindow()
{
    saveSessionToDb();
    m_db->printS(); // delete it

    delete ui;
    delete m_db;
}

void MainWindow::on_actionLight_triggered()
{
    this->setStyleSheet("color: black; background-color: #dde1e5;");
    ui->playlistView->setStyleSheet("QHeaderView::section { background-color:white } color: black; background-color: #dde1e5;");
    ui->pushButtonMute->setStyleSheet("QPushButton:hover:!pressed { background-color: #f5f7fa; }");
    m_musiccontrolinterface->isLightTheme();
    ui->actionDark->setChecked(false);
    ui->actionLight->setChecked(true);

    qDebug() << "light theme";
}

void MainWindow::on_actionDark_triggered()
{
    this->setStyleSheet("color: white; background-color: #323336;");
    ui->playlistView->setStyleSheet("QHeaderView::section { background-color:#323336 } color: white; background-color: #323336;");
    ui->pushButtonMute->setStyleSheet("QPushButton:hover:!pressed { background-color: #7e7f80; }");
    m_musiccontrolinterface->isDarkTheme();
    ui->actionDark->setChecked(true);
    ui->actionLight->setChecked(false);    

    qDebug() << "dark theme";
}
