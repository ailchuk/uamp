#include "libraryformdialog.h"
#include "ui_libraryformdialog.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mytreewidgetitem.h"
#include <QTableWidget>
#include "QInputDialog"

libraryformdialog::libraryformdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::libraryformdialog)
{
    ui->setupUi(this);
    m_main_window = qobject_cast<MainWindow*>(parent);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);

    setupMenu();
}

void libraryformdialog::loadPlaylists()
{
    auto save = m_db->loadPlaylists();

    qDebug() << save.size();
    if (save.size() <= 0)
        return ;

    MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "New playlist", "");
    ui->treeWidget->addTopLevelItem(topLevelItem1);
    topLevelItem1->setText(0, save.begin()->first);

    for (const auto &it : save)
    {
        MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), it.first, it.second);
    }
}


void libraryformdialog::setupMenu() {
    m_menu.addAction("Rename playlist", this, []() {

    });
}

libraryformdialog::~libraryformdialog()
{
    delete ui;
}

void libraryformdialog::on_pushButtonOpenDir_clicked()
{
    QStringList trackPath = QFileDialog::getOpenFileNames(this,
                            tr("Open files"),
                            QString(),
                            tr("Audio Files (*.mp3; *.flac; *.m3u)"));

    foreach (QString filePath, trackPath) {
        if (true/* add SQL querry here */) {
            QString fileName = QDir(filePath).dirName();
            MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), QDir(filePath).dirName(), filePath);
        }
    }
}

void libraryformdialog::on_pushButtonAddTracksFromQueue_clicked()
{
    auto mainPlaylistModel = this->m_main_window->m_playListModel;

    for (auto row : this->m_main_window->ui->playlistView->selectionModel()->selectedRows()) {
        QStandardItem* itemName = mainPlaylistModel->item(row.row());
        QStandardItem* itemPath = mainPlaylistModel->item(row.row(), 1);
        QString trackName = itemName->data(0).toString();
        QString trackPath = itemPath->data(0).toString();

        MyTreeWidgetItem::addToItemHandler(ui->treeWidget->currentItem(), trackName, trackPath);
    }
}

void libraryformdialog::on_pushButtonAddToQueue_clicked()
{
    for (auto row : this->ui->treeWidget->selectedItems()) {
        MyTreeWidgetItem *item = dynamic_cast<MyTreeWidgetItem *>(row);

        QList<QStandardItem *> items;
        items.append(new QStandardItem(item->Name));
        items.append(new QStandardItem(item->Path));

        if (item->Path.begin() != item->Path.end()) {
            this->m_main_window->m_playListModel->appendRow(items);
            this->m_main_window->m_playlist->addMedia(QUrl(item->Path));
            this->m_main_window->m_db->addToQueue(item->Path);
        }
    }
}

void libraryformdialog::on_pushButtonNewPlaylist_clicked()
{
    MyTreeWidgetItem *topLevelItem1 = new MyTreeWidgetItem(ui->treeWidget, "New playlist", "");
    ui->treeWidget->addTopLevelItem(topLevelItem1);
    topLevelItem1->setText(0, "New playlist");
}

void libraryformdialog::on_pushButtonSavePlaylist_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    QMediaPlaylist *playlist = new QMediaPlaylist;
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "*.m3u");

    if (filename != nullptr)
    {
        for (int i = 0; i < item->childCount(); ++i)
        {
            MyTreeWidgetItem *items = dynamic_cast<MyTreeWidgetItem *>(item->child(i));

            playlist->addMedia(QUrl(items->Path));
            qDebug() << QString(items->Path);
        }
        playlist->save(QUrl::fromLocalFile(filename), "m3u");
    }

    m_db->SavePlaylist(filename, item->text(0));

    delete playlist;
}

void libraryformdialog::on_pushButtonDelete_clicked()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if (item)
        item->~QTreeWidgetItem();
}

void libraryformdialog::on_pushButtonRenamePlaylist_clicked()
{

    MyTreeWidgetItem *item = dynamic_cast<MyTreeWidgetItem *>(ui->treeWidget->currentItem());
    if (item->Path.begin() != item->Path.end())
        return;

    QString defaultText("new name");
    bool ok;

    QString caseInput = QInputDialog::getText(this, tr("Rename"), ui->treeWidget->currentItem()->text(0), QLineEdit::Normal, defaultText, &ok);
    if (caseInput != nullptr) {
        ui->treeWidget->currentItem()->setText(0, caseInput);
    }
}
