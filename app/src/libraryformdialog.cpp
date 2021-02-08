#include "libraryformdialog.h"
#include "ui_libraryformdialog.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mytreewidgetitem.h"
#include <QTableWidget>

libraryformdialog::libraryformdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::libraryformdialog)
{
    ui->setupUi(this);
    m_main_window = qobject_cast<MainWindow*>(parent);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

    setupMenu();
}

void libraryformdialog::setupMenu() {
//    m_menu.addAction("", this, []() {

//    });


    connect(ui->pushButtonOpenDir, &QPushButton::clicked, this, [this](){
        QStringList trackPath = QFileDialog::getOpenFileNames(this,
                                tr("Open files"),
                                QString(),
                                tr("Audio Files (*.mp3; *.flac)"));

        foreach (QString filePath, trackPath) {
            if (true/* add SQL querry here */) {
                QString fileName = QDir(filePath).dirName();
                MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), QDir(filePath).dirName(), filePath);
            }
        }
    });

    connect(ui->pushButtonAddTracksFromQueue, &QPushButton::clicked, this, [this](){
        auto mainPlaylistModel = this->m_main_window->m_playListModel;
        foreach(auto row, this->m_main_window->ui->playlistView->selectionModel()->selectedIndexes()) {
            QStandardItem* item = mainPlaylistModel->itemFromIndex(row);
            QString trackPath = item->data(1).toString();
            QString trackName = item->data(0).toString();

            MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), trackName, trackPath);

}
    });

    connect(ui->pushButtonAddToQueue, &QPushButton::clicked, this, [this](){
        auto mainPlaylistModel = this->ui->treeWidget->selectedItems();
        foreach (auto item, this->ui->treeWidget->selectionModel()->selectedIndexes()) {
            QList<QStandardItem *> items;
            QString trackPath = item.data(1).toString();
            QString trackName = item.data(0).toString();
            items.append(new QStandardItem(trackName));
            items.append(new QStandardItem(trackPath));

            this->m_main_window->m_playListModel->appendRow(items);
            this->m_main_window->m_playlist->addMedia(QUrl(trackPath));
//            this->m_main_window->m_db->addToQueue(trackPath);
        }
    });

    connect(ui->pushButtonNewPlaylist, &QPushButton::clicked, this, [this](){
        MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "New playlist", "");
        ui->treeWidget->addTopLevelItem(topLevelItem1);
        topLevelItem1->setText(0, "New playlist");
    });

    connect(ui->pushButtonSavePlaylist, &QPushButton::clicked, this, [this](){

    });

    connect(ui->pushButtonDelete, &QPushButton::clicked, this, [this](){
        QTreeWidgetItem* item = ui->treeWidget->currentItem();
        if (item)
            item->~QTreeWidgetItem();
    });

}

libraryformdialog::~libraryformdialog()
{
    delete ui;
}
