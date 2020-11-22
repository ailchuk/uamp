#include "uamp.h"
#include "./ui_uamp.h"

UAmp::UAmp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UAmp)
{
    ui->setupUi(this);
//    this->setAcceptDrops(true);

    m_playListModel = new QStandardItemModel(this);
    m_player = new QMediaPlayer(this);          // Init player
    m_playlist = new QMediaPlaylist(m_player);  // Init playlist

    ui->songs_table->setModel(m_playListModel);
    ui->songs_table->hideColumn(1);
    ui->songs_table->verticalHeader()->setVisible(false);
    ui->songs_table->horizontalHeader()->setVisible(false);

    m_player->setPlaylist(m_playlist);
    m_player->setVolume(70);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);  // Set circular play mode playlist

    connect(ui->songs_table, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());
    });
}

UAmp::~UAmp()
{
    delete ui;
}

//void UAmp::addNewSong(const QString &path) {

//}


void UAmp::on_b_add_clicked() {
//    if (!m_playlist->isEmpty()) {
    if (m_playListModel->columnCount() > 0) {
        m_playlist->clear();
        m_playListModel->clear();
    }

    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio-Files(*.mp3 *.wav *.mp4 *.flac)"));

        foreach (QString filePath, files) {
            QList<QStandardItem *> items;
            items.append(new QStandardItem(QDir(filePath).dirName()));
            m_playListModel->appendRow(items);
            m_playlist->addMedia(QUrl(filePath));
        }

}

//void UAmp::dragEnterEvent(QDragEnterEvent *e)
//{
//    if (e->mimeData()->hasUrls()) {
//        e->acceptProposedAction();
//    }
//}

//void UAmp::dropEvent(QDropEvent *e)
//{

//    foreach (const QUrl &url, e->mimeData()->urls()) {
//        QList<QStandardItem *> items;

//        items.append(new QStandardItem(url.fileName()));
//        m_playListModel->appendRow(items);
//        m_playlist->addMedia(url);
//    }
//}
